require 'bunny'

class Worker
  def initialize(queue_name = 'min_max_queue')
    setup_connection(queue_name)
  end

  def run
    puts ' [*] Waiting for messages. To exit press CTRL+C'

    subscribe_to_work_queue
  end

  def close_connection
    @connection.close
  end

  private

  def setup_connection(queue_name)
    @connection = Bunny.new(automatically_recover: false)
    @connection.start

    @channel = @connection.create_channel
    @work_queue = @channel.queue(queue_name, durable: true)
    @exchange = @channel.default_exchange

    # Don't give more than one message to a worker at a time
    @channel.prefetch(1)
  end

  def subscribe_to_work_queue
    @work_queue.subscribe(manual_ack: true, block: true) do |delivery_info, properties, body|
      list = body.unpack('f*')

      puts "Received: #{list.length}"

      @exchange.publish(
        find_min_max(list).pack('f*'),
        routing_key: properties.reply_to,
        correlation_id: properties.correlation_id
      )

      @channel.ack(delivery_info.delivery_tag)
    end
  end

  def find_min_max(list)
    min = list.first
    max = list.first

    list.each do |el|
      min = el if el < min

      max = el if el > max
    end

    [min, max]
  end
end

worker = Worker.new

begin
  worker.run
rescue Interrupt => _
  worker.close_connection
end
