# This is the Compass configuration file.
# The configuration file for nanoc is named “config.yaml”.

project_path     = File.dirname(__FILE__)
http_path        = '/'
output_style     = :compressed
sass_dir         = 'content/doc/style'
images_dir       = 'content/doc/images'

# use SCSS:
sass_options = {
  :syntax => :scss
}
