<?php
/**
 * @file
 * A Just a simple command that calls a bunch of other drush commands.
 */


/**
 * Implements hook_drush_command().
 */
function build_drush_command() {
  $items = array();

  $items['build'] = array(
    'description' => '',
    'callback' => '_drush_build',
    'examples' => array(
      'drush build' => 'Runs a group of drush commands.',
    ),
  );

  return $items;
}

/**
 * Executes the build commands.
 */
function _drush_build() {
  drush_invoke('updatedb');
  drush_invoke('features-revert-all', array('force' => TRUE));
  drush_invoke('cc', array('type' => 'all'));
  drush_log(dt('Built!'), 'success');
}
