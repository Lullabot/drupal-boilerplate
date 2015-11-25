<?php

/**
 * @file
 * Build and deployment commands.
 */

/**
 * Implements hook_drush_command().
 */
function build_drush_command() {
  $items = array();

  $items['build'] = array(
    'description' => 'Runs database updates, reverts features and clears caches.',
    'callback' => '_drush_build',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );

  $items['devify'] = array(
    'description' => 'Configures the current database for development.',
    'callback' => '_drush_devify',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'options' => array(
      'enable-modules' => 'A comma separated list of modules to enable.',
      'disable-modules' => 'A comma separated list of modules to disable.',
      'delete-variables' => 'A comma separated list of variables to delete.',
      'reset-variables' => 'A comma separated list of variables to reset with ' .
                           'the format foo=var,hey=ho.',
    ),
    'examples' => array(
      'drush devify' => 'Uses command default values to set up a database for development.',
      'drush devify --enable-modules=xhprof,devel' => 'Enables XHProf and Devel modules',
      'drush devify --reset-variables=site_mail=local@local.com,file_temporary_path=/tmp'
        => 'Resets site_mail and file_temporary_path variables.',
    ),
  );

  return $items;
}

/**
 * Executes the build commands.
 */
function _drush_build() {
  $commands = array();

  // Run a registry rebuild at first, then do a full bootstrap. Otherwise,
  // this command will fail if the registry has errors.
  drush_invoke_process('@self', 'registry-rebuild', array(), array('no-cache-clear' => TRUE));
  drush_bootstrap(DRUSH_BOOTSTRAP_DRUPAL_FULL);

  $commands[] = array(
    'command' => 'updatedb',
    'options' => array('yes' => TRUE),
  );
  $commands[] = array(
    'command' => 'cache-clear',
    'arguments' => array('drush'),
  );
  $commands[] = array(
    'command' => 'features-revert-all',
    'options' => array('yes' => TRUE),
  );
  $commands[] = array(
    'command' => 'cache-clear',
    'arguments' => array('all'),
  );
  $commands[] = array(
    'command' => 'cron',
  );

  // Pass the drushrc file through to drush_invoke_process
  $default_options = array();
  if ($config = drush_get_option('config-file')) {
    $default_options['config'] = $config;
  }

  foreach ($commands as $command) {
    $command += array('alias' => '@self', 'arguments' => array(), 'options' => array());
    $command['options'] += $default_options;

    drush_invoke_process($command['alias'], $command['command'], $command['arguments'], $command['options']);
    if (drush_get_error() != DRUSH_SUCCESS) {
      return drush_set_error('BUILD_FAILED', dt('Build failed on drush @command.', array('@command' => $command['command'])));
    }
  }

  drush_log(dt('Build completed.'), 'success');
}

/**
 * Callback for drush devify.
 */
function _drush_devify() {
  // Disable modules.
  $modules = drush_get_option('disable-modules', array('update'));
  if (is_string($modules)) {
    $modules = explode(',', $modules);
  }
  drush_invoke('dis', $modules);

  // Enable modules.
  $modules = drush_get_option('enable-modules', array());
  if (is_string($modules)) {
    $modules = explode(',', $modules);
  }
  drush_invoke('en', $modules);

  // Delete variables.
  $variables = drush_get_option('delete-variables', array(
    'googleanalytics_account',
  ));
  if (is_string($variables)) {
    $variables = explode(',', $variables);
  }
  foreach ($variables as $variable) {
    variable_del($variable);
  }

  // Reset variables.
  $variables = drush_get_option('reset-variables', array(
    'preprocess_css' => 0,
    'preprocess_js' => 0,
    'cache' => 0,
    'page_compression' => 0,
  ));
  if (is_string($variables)) {
    // Explode a=b,c=d into an associative array.
    $variables_raw = explode(',', $variables);
    $variables = array();
    foreach ($variables_raw as $value) {
      $variable = explode('=', $value);
      if (count($variable) == 2) {
        $variables[$variable[0]] = $variable[1];
      }
    }
  }
  foreach ($variables as $name => $value) {
    drush_invoke('vset', array($name, $value));
  }

  // Sanitize database.
  // @see sql_drush_sql_sync_sanitize() to add your sanitize queries.
  drush_invoke('sql-sanitize');

  drush_log(dt('Devified!'), 'success');
}
