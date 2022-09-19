srand(Time.now.to_i)

bytes_to_write = 10 << 20 # 10 MB
bytes_written = 0

File.open('input_big_two.txt', 'w') do |file|
  file.write("2 2 2\n")

  loop do
    break if bytes_written >= bytes_to_write

    (2..5).each do |length|
      str = ""

      (1..length).each do |i|
        if i == 1
          str += "#{rand(1..9)}"
        else
          str += "#{rand(0..9)}"
        end
      end

      bytes_written += file.write("#{str}\n")
    end
  end
end

puts "#{bytes_written} written"
