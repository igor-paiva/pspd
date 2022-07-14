require 'bunny'
require 'thread'
require 'securerandom'

class Publisher
  DEFAULT_ARRAY_SLICES = 2
  DEFAULT_ARRAY_LENGTH = 500_000

  attr_reader :min, :max

  def initialize(slices, size, queue_name = 'min_max_queue')
    @size = size || DEFAULT_ARRAY_LENGTH
    @slices = slices || DEFAULT_ARRAY_SLICES

    @list = rand_float_array(@size)

    setup_connection(queue_name)
  end

  def send_messages
    @slices.times do |i|
      l = (i * @size) / @slices
      r = ((i + 1) * @size) / @slices

      @work_queue.publish(@list[l..r].pack('f*'), persistent: true, correlation_id: @request_ids[i], reply_to: @reply_queue.name)
    end

    wait_for_replies
  end

  def close_connection
    @connection.close
  end

  private

  def setup_connection(queue_name)
    @connection = Bunny.new(automatically_recover: false)
    @connection.start

    @lock = Mutex.new
    @channel = @connection.create_channel
    @work_queue = @channel.queue(queue_name, durable: true)

    setup_reply_queue
  end

  def setup_reply_queue
    @min = @list.first
    @max = @list.first
    @answered_requests = 0
    @reply_queue = @channel.queue('', exclusive: true)
    @request_ids = Array.new(@slices) { SecureRandom.uuid }

    @reply_queue.subscribe do |_delivery_info, properties, payload|
      if @request_ids.include?(properties[:correlation_id])
        slice_min, slice_max = payload.unpack('f*')

        @lock.synchronize {
          @min = slice_min if slice_min < @min

          @max = slice_max if slice_max > @max

          @answered_requests += 1
        }
      end
    end
  end

  def wait_for_replies
    while @answered_requests < @slices
      sleep(microseconds(15))
    end
  end

  def f_aleat(x)
    rand(0..x.to_f)
  end

  def rand_float_array(n)
    srand(Time.now.to_i)

    Array.new(n) do |i|
      Math.sqrt((i - (f_aleat(n) / 2)) ** 2)
    end
  end

  def microseconds(value)
    value.to_f / 1_000_000.0
  end
end

publisher = Publisher.new(ARGV[0]&.to_i, ARGV[1]&.to_i)

publisher.send_messages
publisher.close_connection

puts "Min: #{publisher.min}\nMax: #{publisher.max}"
