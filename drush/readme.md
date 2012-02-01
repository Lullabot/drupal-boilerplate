#Drush configurations

##How to use this directory

Drush doesn't by default know to search this directory. To work around that we need
to add this awesome snippet to our local drushrc.php file.

    // Load a drushrc.php file from the 'drush' folder at the root
    // of the current git repository. Customize as desired.
    // (Script by grayside; @see: http://grayside.org/node/93)
    exec('git rev-parse --git-dir 2> /dev/null', $output);
    if (!empty($output)) {
      $repo = $output[0];
      $options['config'] = $repo . '/../drush/drushrc.php';
      $options['include'] = $repo . '/../drush/commands';
      $options['alias-path'] = $repo . '/../drush/aliases';
    }

Once the above snippet in in your drushrc.php file then drush will know to read our
custom drushrc.php and to search our commands and aliases directory for commands
and aliases.


###Commands


###Aliases
