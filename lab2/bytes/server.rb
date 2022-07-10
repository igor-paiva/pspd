require 'grpc'
require_relative 'min_max_bytes_services_pb'

class MinMaxServer < MinMax::Service
  def min_max_calc(sum_req, _unused_call)
    list = sum_req.list.unpack('f*')
    min = list[0]
    max = list[0]

    list.each do |item|
      min = item if item < min

      max = item if item > max
    end

    MinMaxReply.new(min: min, max: max)
  end
end

def main
  s = GRPC::RpcServer.new
  s.add_http2_port('0.0.0.0:5001', :this_port_is_insecure)
  s.handle(MinMaxServer)
  # Runs the server with SIGHUP, SIGINT and SIGQUIT signal handlers to
  #   gracefully shutdown.
  # User could also choose to run server via call to run_till_terminated
  s.run_till_terminated_or_interrupted([1, 'int', 'SIGQUIT'])
end

main
