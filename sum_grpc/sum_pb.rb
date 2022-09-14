# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: sum.proto

require 'google/protobuf'

Google::Protobuf::DescriptorPool.generated_pool.build do
  add_file("sum.proto", :syntax => :proto3) do
    add_message "SumByteArrayParam" do
      optional :length, :uint32, 1
      optional :list, :bytes, 2
    end
    add_message "SumArrayParam" do
      optional :value, :float, 1
    end
    add_message "SumParams" do
      optional :x, :float, 1
      optional :y, :float, 2
    end
    add_message "SumReply" do
      optional :result, :float, 1
    end
  end
end

SumByteArrayParam = ::Google::Protobuf::DescriptorPool.generated_pool.lookup("SumByteArrayParam").msgclass
SumArrayParam = ::Google::Protobuf::DescriptorPool.generated_pool.lookup("SumArrayParam").msgclass
SumParams = ::Google::Protobuf::DescriptorPool.generated_pool.lookup("SumParams").msgclass
SumReply = ::Google::Protobuf::DescriptorPool.generated_pool.lookup("SumReply").msgclass