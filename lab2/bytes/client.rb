require 'grpc'
require_relative 'min_max_bytes_services_pb'

ARRAY_LENGTH = 500_000

def f_aleat(x)
  rand(0..x.to_f)
end

def rand_float_array(n)
  Array.new(n) do |i|
    Math.sqrt((i - (f_aleat(n) / 2)) ** 2)
  end
end

def main
  hostname = ARGV.size > 0 ?  ARGV[0] : 'localhost:5001'

  stub = MinMax::Stub.new(hostname, :this_channel_is_insecure)

  begin
    puts "antes de gerar o array"
    list = rand_float_array(ARRAY_LENGTH)
    puts "depois de gerar o array"

    puts "Min: #{list.min}"
    puts "Max: #{list.max}"

    puts "antes de mandar a request"
    reply = stub.min_max_calc(MinMaxParams.new(list: list.pack('f*')))
    puts "depois de mandar a request\n"

    p "Min: #{reply.min}"
    p "Max: #{reply.max}"
  rescue GRPC::BadStatus => e
    abort "ERROR: #{e.message}"
  end
end

main
