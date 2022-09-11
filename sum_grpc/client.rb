require 'grpc'
require_relative 'sum_services_pb'

def main
  hostname = ARGV[0]
  x = ARGV[2]
  y = ARGV[3]

  stub = Sum::Stub.new(hostname, :this_channel_is_insecure)

  begin
    # result = stub.sum_calc(SumParams.new(x: x.to_f, y: y.to_f)).result
    # p "Result: #{result}"

    # list = Array.new(10) { |i| SumArrayParam.new(value: i + 1) }
    list = Array.new(500_000) { |i| (i + 1).to_f }

    # result = stub.sum_array_calc(list).result
    # p "Result: #{result}"

    param = SumByteArrayParam.new(length: 10, list: list.pack('f*'))

    result = stub.sum_byte_array_calc(param).result
    p "Result: #{result}"
  rescue GRPC::BadStatus => e
    abort "ERROR: #{e.message}"
  end
end

main
