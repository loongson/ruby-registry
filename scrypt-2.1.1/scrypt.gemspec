# -*- encoding: utf-8 -*-
# stub: scrypt 2.1.1 ruby lib
# stub: ext/scrypt/Rakefile

Gem::Specification.new do |s|
  s.name = "scrypt".freeze
  s.version = "2.1.1"

  s.required_rubygems_version = Gem::Requirement.new(">= 0".freeze) if s.respond_to? :required_rubygems_version=
  s.require_paths = ["lib".freeze]
  s.authors = ["Patrick Hogan".freeze, "Stephen von Takach".freeze]
  s.cert_chain = ["-----BEGIN CERTIFICATE-----\nMIIDvDCCAqSgAwIBAgIBATANBgkqhkiG9w0BAQUFADBaMQ4wDAYDVQQDDAVzdGV2\nZTEfMB0GCgmSJomT8ixkARkWD2FkdmFuY2VkY29udHJvbDETMBEGCgmSJomT8ixk\nARkWA2NvbTESMBAGCgmSJomT8ixkARkWAmF1MB4XDTE1MDYwNDA3NDEwOFoXDTE2\nMDYwMzA3NDEwOFowWjEOMAwGA1UEAwwFc3RldmUxHzAdBgoJkiaJk/IsZAEZFg9h\nZHZhbmNlZGNvbnRyb2wxEzARBgoJkiaJk/IsZAEZFgNjb20xEjAQBgoJkiaJk/Is\nZAEZFgJhdTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBANH4dCGtSx9g\nOdtlhH5NlUJi0Yuxc+avZ4lxfySR/sqSysnTjrA1z7/iXdo6guS5Q70IB/k4wxhB\nHhgY0Pd3kZ+CVkOatBu+5CTprij6M/uvIR0x+MDtvOZl30UJzUnt0r2b1Lj9hEuL\nnb40GXdc16m09vxOZRCjIpV9Q1M2oT9eCkx21D/pLWnw293F24e5GtQ8GPNR4tW/\n9ZjJV976joP8b8XgE2n3/BhqOpyggvPTGu6X/LQMvZhDmpHjRKNoBsDNUmGqr29T\nHMvym6ZC25beUC89IA3PdkbSAh/bbHjNx2pGMjM8hXlibpTbaPP6FDfMBMHdCsva\nfKqTV01U2kUCAwEAAaOBjDCBiTAJBgNVHRMEAjAAMAsGA1UdDwQEAwIEsDAdBgNV\nHQ4EFgQUILtSC8FKZsfXCgEVnGba+1jB0vUwJwYDVR0RBCAwHoEcc3RldmVAYWR2\nYW5jZWRjb250cm9sLmNvbS5hdTAnBgNVHRIEIDAegRxzdGV2ZUBhZHZhbmNlZGNv\nbnRyb2wuY29tLmF1MA0GCSqGSIb3DQEBBQUAA4IBAQArjqxJ6GjmOJGPGN1haQCr\n9vI6nET//Vcz5tbW/XiBCDtQg0/df9mMzWIj58mbimL5s0sPkyweAiAYX4VXPD+W\nDVsuOhBigrJuAvVdG/3HkrQCBVGQNV1Z8Q561ECChzamRQ6Uij642R8z/UTCOot9\ntobIIzKL9Zg2w2hs4Od0feHapcKMMFsCb2ODwuLXKHFyE6IdZviuORkRdNE4QQ04\n5Bi3W9Xf1BH2VODGXbWJ/7Wa1hBfmxXeWxat27WlvW3xFTi4NaHMlp+l3wa1gTN6\nXm3vXPA+7+FFynIH9Fw2NiURj9auCa2HIRp63V0TGhrBSxuB7e2qZhKHVt2Jnk+o\n-----END CERTIFICATE-----\n".freeze]
  s.date = "2016-02-25"
  s.description = "    The scrypt key derivation function is designed to be far\n    more secure against hardware brute-force attacks than\n    alternative functions such as PBKDF2 or bcrypt.\n".freeze
  s.email = ["pbhogan@gmail.com".freeze, "steve@advancedcontrol.com.au".freeze]
  s.extensions = ["ext/scrypt/Rakefile".freeze]
  s.files = ["COPYING".freeze, "README.md".freeze, "Rakefile".freeze, "autotest/discover.rb".freeze, "ext/scrypt/Rakefile".freeze, "ext/scrypt/crypto_scrypt-sse.c".freeze, "ext/scrypt/crypto_scrypt.h".freeze, "ext/scrypt/memlimit.c".freeze, "ext/scrypt/memlimit.h".freeze, "ext/scrypt/scrypt_calibrate.c".freeze, "ext/scrypt/scrypt_calibrate.h".freeze, "ext/scrypt/scrypt_ext.c".freeze, "ext/scrypt/scrypt_ext.h".freeze, "ext/scrypt/scrypt_platform.h".freeze, "ext/scrypt/scryptenc_cpuperf.c".freeze, "ext/scrypt/scryptenc_cpuperf.h".freeze, "ext/scrypt/sha256.c".freeze, "ext/scrypt/sha256.h".freeze, "ext/scrypt/sysendian.h".freeze, "lib/scrypt.rb".freeze, "lib/scrypt/scrypt_ext.rb".freeze, "lib/scrypt/security_utils.rb".freeze, "lib/scrypt/version.rb".freeze, "scrypt.gemspec".freeze, "spec/scrypt/engine_spec.rb".freeze, "spec/scrypt/password_spec.rb".freeze, "spec/scrypt/utils_spec.rb".freeze, "spec/spec_helper.rb".freeze]
  s.homepage = "https://github.com/pbhogan/scrypt".freeze
  s.licenses = ["MIT".freeze]
  s.rubygems_version = "3.3.26".freeze
  s.summary = "scrypt password hashing algorithm.".freeze
  s.test_files = ["spec/scrypt/engine_spec.rb".freeze, "spec/scrypt/password_spec.rb".freeze, "spec/scrypt/utils_spec.rb".freeze, "spec/spec_helper.rb".freeze]

  if s.respond_to? :specification_version then
    s.specification_version = 4
  end

  if s.respond_to? :add_runtime_dependency then
    s.add_runtime_dependency(%q<ffi-compiler>.freeze, [">= 0.0.2"])
    s.add_runtime_dependency(%q<rake>.freeze, [">= 0"])
    s.add_development_dependency(%q<rspec>.freeze, [">= 0"])
    s.add_development_dependency(%q<rdoc>.freeze, [">= 0"])
    s.add_development_dependency(%q<awesome_print>.freeze, [">= 0"])
  else
    s.add_dependency(%q<ffi-compiler>.freeze, [">= 0.0.2"])
    s.add_dependency(%q<rake>.freeze, [">= 0"])
    s.add_dependency(%q<rspec>.freeze, [">= 0"])
    s.add_dependency(%q<rdoc>.freeze, [">= 0"])
    s.add_dependency(%q<awesome_print>.freeze, [">= 0"])
  end
end

