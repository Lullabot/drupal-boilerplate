# Require any gem-installed Sass modules on your system.
require 'susy'
require 'breakpoint'
require 'sass-globbing'

# Location of the theme's resources.
css_dir         = "css"
sass_dir        = "scss"
images_dir      = "images"
javascripts_dir = "js"
fonts_dir       = "fonts"

# Change this to :production when ready to deploy the CSS to the live server.
environment = :production

# You can select your preferred output style here (can be overridden via the command line):
# output_style = :expanded or :nested or :compact or :compressed
output_style    = :compressed

# To enable relative paths to assets via compass helper functions. Uncomment:
relative_assets = true

# To disable debugging comments that display the original location of your selectors. Uncomment:
line_comments   = false

# To enable sourcemap support. Uncomment:
#sourcemap       = true
