require 'bunny'
require 'securerandom'

DEFAULT_ARRAY_SLICES = 2
DEFAULT_ARRAY_LENGTH = 500_000

def f_aleat(x)
  rand(0..x.to_f)
end

def rand_float_array(n)
  srand(Time.now.to_i)

  Array.new(n) do |i|
    Math.sqrt((i - (f_aleat(n) / 2)) ** 2)
  end
end

def find_min_max(list)
  min = list.first
  max = list.first

  list.each do |el|
    min = el if el < min

    max = el if el > max
  end

  return min, max
end

def microseconds(value)
  value.to_f / 1_000_000.0
end

def main
  connection = Bunny.new(automatically_recover: false)
  connection.start

  list_slices = ARGV[0]&.to_i || DEFAULT_ARRAY_SLICES
  array_length = ARGV[1]&.to_i || DEFAULT_ARRAY_LENGTH

  received_ids = 0
  list = rand_float_array(array_length)
  request_ids = Array.new(list_slices) { SecureRandom.uuid }

  min = list.first
  max = list.first

  channel = connection.create_channel
  work_queue = channel.queue('min_max_queue', durable: true)
  reply_queue = channel.queue('', exclusive: true)

  reply_queue.subscribe do |_delivery_info, properties, payload|
    if request_ids.include?(properties[:correlation_id])
      slice_min, slice_max = payload.unpack('f*')

      min = slice_min if slice_min < min

      max = slice_max if slice_max > max

      received_ids += 1
    end
  end

  list_slices.times do |i|
    l = (i * array_length) / list_slices
    r = ((i + 1) * array_length) / list_slices

    work_queue.publish(list[l..r].pack('f*'), persistent: true, correlation_id: request_ids[i], reply_to: reply_queue.name)
  end

  while received_ids < list_slices
    sleep(microseconds(15))
  end

  puts "Min: #{min}\nMax: #{max}"

  connection.close
end

main
