require 'grpc'
require_relative 'min_max_services_pb'

class MinMaxServer < MinMax::Service
  def min_max_calc(call)
    # min = nil
    # max = nil
    min = Float::MAX
    max = Float::MIN

    call.each_remote_read do |element|
      # if min.nil? && max.nil?
      #   min = element.value
      #   max = element.value
      # else
      min = element.value if element.value < min

      max = element.value if element.value > max
      # end
    end

    MinMaxReply.new(min: min, max: max)
  end
end

def main
  s = GRPC::RpcServer.new
  s.add_http2_port('0.0.0.0:5000', :this_port_is_insecure)
  s.handle(MinMaxServer)
  # Runs the server with SIGHUP, SIGINT and SIGQUIT signal handlers to
  #   gracefully shutdown.
  # User could also choose to run server via call to run_till_terminated
  s.run_till_terminated_or_interrupted([1, 'int', 'SIGQUIT'])
end

main
