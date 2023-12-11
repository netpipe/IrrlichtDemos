CURRENT_ENV = ENV['os']

namespace "ci" do
	desc "Builds and runs tests for CI Server"
	task :all => [:build, :tests] do
	end

	desc "Continous Integration Build Task"
	task :build do
		puts "Starting CI Build on #{CURRENT_ENV}."
		if CURRENT_ENV == "Windows_NT"
			sh "codeblocks --rebuild *.workspace --target=\"Win32\""		
		else
			sh "codeblocks --rebuild *.workspace --target=\"Linux\""
		end
	end
	desc "Run tests in the continuous integration env"
	task :tests do
		puts "Running Tests"
		sh "tests/bin/desteer.exe"
	end
end
