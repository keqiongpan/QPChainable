#
# Be sure to run `pod lib lint QPChainable.podspec' to ensure this is a
# valid spec before submitting.
#
# Any lines starting with a # are optional, but their use is encouraged
# To learn more about a Podspec see https://guides.cocoapods.org/syntax/podspec.html
#

Pod::Spec.new do |s|
  s.name             = 'QPChainable'
  s.version          = '1.2.0'
  s.summary          = 'Provides an easy way to construct chained expressions for Objective-C.'

# This description is used to generate tags and improve search results.
#   * Think: What does it do? Why did you write it? What is the focus?
#   * Try to keep it short, snappy and to the point.
#   * Write the description between the DESC delimiters below.
#   * Finally, don't worry about the indent, CocoaPods strips it!

  # s.description      = 'Provides an easy way to construct chained expressions for Objective-C.'

  s.homepage         = 'https://github.com/keqiongpan/QPChainable'
  # s.screenshots     = 'www.example.com/screenshots_1', 'www.example.com/screenshots_2'
  s.license          = { :type => 'MIT', :file => 'LICENSE' }
  s.author           = { 'Qiongpan Ke' => 'keqiongpan@163.com' }
  s.source           = { :git => 'https://github.com/keqiongpan/QPChainable.git', :tag => s.version.to_s }
  # s.social_media_url = 'https://twitter.com/<TWITTER_USERNAME>'

  s.ios.deployment_target = '8.0'
  s.osx.deployment_target = '10.8'
  s.tvos.deployment_target = '9.0'
  s.watchos.deployment_target = '2.0'

  s.source_files = 'QPChainable'

  # s.resource_bundles = {
  #   'QPChainable' => ['QPChainable/Assets/*.png']
  # }

  s.public_header_files = 'QPChainable/**/*.h'
  s.frameworks = 'Foundation'
  # s.dependency 'AFNetworking', '~> 2.3'
end
