require 'grpc'
require_relative 'sum_services_pb'

class SumServer < Sum::Service
  def sum_calc(sum_req, _unused_call)
    ::SumReply.new(result: sum_req.x + sum_req.y)
  end

  def sum_array_calc(call)
    sum = 0

    call.each_remote_read do |param|
      sum += param.value
    end

    ::SumReply.new(result: sum)
  end

  def sum_byte_array_calc(sum_req, _unused_call)
    list = sum_req.list.unpack('f*')

    SumReply.new(result: list.sum)
  end
end

# main starts an RpcServer that receives requests to GreeterServer at the sample
# server port.
def main
  s = GRPC::RpcServer.new
  s.add_http2_port('0.0.0.0:50051', :this_port_is_insecure)
  s.handle(SumServer)
  # Runs the server with SIGHUP, SIGINT and SIGQUIT signal handlers to
  #   gracefully shutdown.
  # User could also choose to run server via call to run_till_terminated
  s.run_till_terminated_or_interrupted([1, 'int', 'SIGQUIT'])
end

main
