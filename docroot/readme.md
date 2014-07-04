# docroot
This is the directory where Drupal core will be installed.

Your web server should point to this directory as the root.

There is an install profile available that will give you some basic editorial tools. Feel free to remove everything in this folder and start fresh if you wish.

## Install Profile Instructions
```
$ cd docroot
$ drush make lullabot_boilerplate.make
```

Then install Drupal, selecting the "Lullabot Boilerplate" profile. This will give you

* Responsive toolbar
* Responsive admin theme (named "yoursite_shiny" - please edit this for your own site)
*  CKEditor with a HTML Purifier based filter and Linkit
*  Basic "Page" content type

