# Generated by the protocol buffer compiler.  DO NOT EDIT!
# Source: min_max_bytes.proto for package ''

require 'grpc'
require_relative 'min_max_bytes_pb'

module MinMax
  class Service

    include ::GRPC::GenericService

    self.marshal_class_method = :encode
    self.unmarshal_class_method = :decode
    self.service_name = 'MinMax'

    rpc :MinMaxCalc, ::MinMaxParams, ::MinMaxReply
  end

  Stub = Service.rpc_stub_class
end
