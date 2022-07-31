require 'faker'

classes = [
  { klass: Faker::Name, methods: %i[first_name middle_name] },
  { klass: Faker::Movies::BackToTheFuture, methods: %i[character] },
  { klass: Faker::Movies::LordOfTheRings, methods: %i[character] },
  { klass: Faker::Movies::StarWars, methods: %i[character] },
  { klass: Faker::Movies::Hobbit, methods: %i[character] },
  { klass: Faker::TvShows::RickAndMorty, methods: %i[character] }
]

MAX_FRIENDS_PER_LINE = 15

megabytes_to_write = ARGV.size > 0 ?  ARGV[0].to_i : 250
output_filename = ARGV.size > 1 ?  ARGV[1] : 'input.txt'

bytes_to_write = megabytes_to_write << 20

bytes_written = 0

srand(Time.now.to_i)

def random_person(classes)
  data = classes.sample
  method = data[:methods].sample

  person = data[:klass].send(method)

  return person.split.first if person.include?(' ')

  person
end

File.open(output_filename, 'w') do |file|
  loop do
    break if bytes_written >= bytes_to_write

    person = random_person(classes)

    file.write(person)

    num_friends = rand(1..MAX_FRIENDS_PER_LINE)

    num_friends.times do
      bytes_written += file.write(", #{random_person(classes)}")
    end

    file.write("\n")
  end
end

puts "#{bytes_written} written"
