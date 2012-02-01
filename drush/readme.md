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

###Aliases
The aliases directory is used to store aliases specific to your project. This is a great
place to share aliases such as _@example.staging_, _@example.live_, _@example.rc_ etc..

Be cautious about not storing local specific alias because the probably wont work in
every environment.

###Commands
The commands directory is used to store drush commands you would like to share
with your entire team. This is a great place for your custom drush xyz command.

By default we include the __Registry Rebuild__ and the __Build__ command.

####Registry Rebuild
Instead of trying to explain what it does. Here's a snippet from its [project
page](http://drupal.org/project/registry_rebuild).

>>There are times in Drupal 7 when the registry gets hopelessly hosed and you need to rebuild the registry
 (a list of PHP classes and the files they go with). Sometimes, though, you can't do this regular
 cache-clear activity because some class is required when the system is trying to bootstrap.

####Build
The build command is nothing but a simple drush commands that calls other drush commands
such as updatedb, features-revert-all, and cache-clear. The reason for the build command
is to guarantee your deployment is always being executed in the way you intended. Here's
what the drush command essentially translates to.

    drush updatedb
    drush features-revert-all --force
    drush cc all

But instead of of calling all those commands in the same order all the time you can now
call _drush build --yes_.
    