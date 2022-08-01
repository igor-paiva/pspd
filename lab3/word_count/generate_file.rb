require 'faker'

methods = %i[famous_last_words most_interesting_man_in_the_world jack_handey matz robin singular_siegler yoda]

output_filename = ARGV.size > 0 ?  ARGV[0] : 'input.txt'
megabytes_to_write = ARGV.size > 1 ?  ARGV[1].to_i : 250

bytes_to_write = megabytes_to_write << 20

bytes_written = 0

srand(Time.now.to_i)

File.open(output_filename, 'w') do |file|
  loop do
    break if bytes_written >= bytes_to_write

    methods.each do |method_name|
      quote = " #{Faker::Quote.send(method_name)}"

      quote.gsub!(/[^a-zA-Z\s]/, ' ')

      bytes_written += file.write(quote)
    end

    file.write("\n") if rand(0..9) % 2
  end
end

puts "#{bytes_written} written"
