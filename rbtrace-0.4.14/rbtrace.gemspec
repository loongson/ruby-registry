# -*- encoding: utf-8 -*-
# stub: rbtrace 0.4.14 ruby lib ext
# stub: ext/extconf.rb

Gem::Specification.new do |s|
  s.name = "rbtrace".freeze
  s.version = "0.4.14"

  s.required_rubygems_version = Gem::Requirement.new(">= 0".freeze) if s.respond_to? :required_rubygems_version=
  s.require_paths = ["lib".freeze, "ext".freeze]
  s.authors = ["Aman Gupta".freeze]
  s.date = "2020-08-03"
  s.description = "rbtrace shows you method calls happening inside another ruby process in real time.".freeze
  s.email = "aman@tmm1.net".freeze
  s.executables = ["rbtrace".freeze]
  s.extensions = ["ext/extconf.rb".freeze]
  s.files = [".gitignore".freeze, "Gemfile".freeze, "LICENSE".freeze, "README.md".freeze, "Rakefile".freeze, "bin/rbtrace".freeze, "ext/.gitignore".freeze, "ext/extconf.rb".freeze, "ext/rbtrace.c".freeze, "ext/src/msgpack-1.1.0.tar.gz".freeze, "lib/rbtrace/cli.rb".freeze, "lib/rbtrace/core_ext.rb".freeze, "lib/rbtrace/interactive/irb.rb".freeze, "lib/rbtrace/interactive/rib.rb".freeze, "lib/rbtrace/memory_report.rb".freeze, "lib/rbtrace/msgq.rb".freeze, "lib/rbtrace/rbtracer.rb".freeze, "lib/rbtrace/version.rb".freeze, "rbtrace.gemspec".freeze, "server.rb".freeze, "test.sh".freeze, "tracers/activerecord.tracer".freeze, "tracers/eventmachine.tracer".freeze, "tracers/io.tracer".freeze, "tracers/mongo.tracer".freeze, "tracers/redis.tracer".freeze, "tracers/testunit.tracer".freeze, "tracers/unicorn.tracer".freeze]
  s.homepage = "http://github.com/tmm1/rbtrace".freeze
  s.licenses = ["MIT".freeze]
  s.rubygems_version = "3.3.7".freeze
  s.summary = "rbtrace: like strace but for ruby code".freeze

  if s.respond_to? :specification_version then
    s.specification_version = 4
  end

  if s.respond_to? :add_runtime_dependency then
    s.add_runtime_dependency(%q<ffi>.freeze, [">= 1.0.6"])
    s.add_runtime_dependency(%q<optimist>.freeze, [">= 3.0.0"])
    s.add_runtime_dependency(%q<msgpack>.freeze, [">= 0.4.3"])
    s.add_development_dependency(%q<rake>.freeze, ["~> 10.0"])
  else
    s.add_dependency(%q<ffi>.freeze, [">= 1.0.6"])
    s.add_dependency(%q<optimist>.freeze, [">= 3.0.0"])
    s.add_dependency(%q<msgpack>.freeze, [">= 0.4.3"])
    s.add_dependency(%q<rake>.freeze, ["~> 10.0"])
  end
end

