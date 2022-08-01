require 'faker'

classes = [
  [Faker::Name, :first_name],
  [Faker::Movies::BackToTheFuture, :character],
  [Faker::Movies::LordOfTheRings, :character],
  [Faker::Movies::StarWars, :character],
  [Faker::Movies::Hobbit, :character],
  [Faker::TvShows::RickAndMorty, :character]
]

MAX_FRIENDS_PER_LINE = 15

output_filename = ARGV.size > 0 ?  ARGV[0] : 'input.txt'
megabytes_to_write = ARGV.size > 1 ?  ARGV[1].to_i : 250

bytes_to_write = megabytes_to_write << 20

bytes_written = 0

srand(Time.now.to_i)

def random_person(classes)
  klass, method = classes.sample

  klass.send(method)
end

File.open(output_filename, 'w') do |file|
  loop do
    break if bytes_written >= bytes_to_write

    person = random_person(classes)

    bytes_written += file.write(person)

    num_friends = rand(1..MAX_FRIENDS_PER_LINE)

    num_friends.times do
      bytes_written += file.write(", #{random_person(classes)}")
    end

    file.write("\n")
  end
end

puts "#{bytes_written} written"
