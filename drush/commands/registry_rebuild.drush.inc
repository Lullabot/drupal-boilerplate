<?php
/**
 * @file
 * Provide Drush integration for release building and dependency building.
 */

/**
 * Implements hook_drush_help().
 */
function registry_rebuild_drush_help($section) {
  switch ($section) {
    case 'drush:registry-rebuild':
      return dt('Rebuild the registry or module cache in a Drupal install.');
  }
}

/**
 * Implements hook_drush_command().
 */
function registry_rebuild_drush_command() {
  $items = array();

  $items['registry-rebuild'] = array(
    'description' => 'Rebuild the registry table (for classes) and the system table (for module locations) in a Drupal install.',
    'callback' => 'drush_registry_rebuild',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap.
    'options' => array(
      'no-cache-clear' => 'Rebuild the registry only, do not clear caches.',
    ),
    'examples' => array(
      'drush rr --no-cache-clear' => 'Rebuild the registry only, do not clear caches.',
    ),
    'aliases' => array('rr'),
  );

  return $items;
}

/**
 * Rebuild the registry.
 *
 * Before calling this we need to be bootstrapped to DRUPAL_BOOTSTRAP_DATABASE.
 */
function drush_registry_rebuild() {
  define('MAINTENANCE_MODE', 'update');
  ini_set('memory_limit', -1);
  if (!drush_bootstrap_to_phase(DRUSH_BOOTSTRAP_DRUPAL_DATABASE)) {
    return drush_set_error('DRUPAL_SITE_NOT_FOUND', dt('You need to specify an alias or run this command within a drupal site.'));
  }
  $include_dir = DRUPAL_ROOT . '/includes';
  $module_dir = DRUPAL_ROOT . '/modules';
  // Use core directory if it exists.
  if (file_exists(DRUPAL_ROOT . '/core/includes/bootstrap.inc')) {
    $include_dir = DRUPAL_ROOT . '/core/includes';
    $module_dir = DRUPAL_ROOT . '/core/modules';
  }

  $includes = array(
    $include_dir . '/bootstrap.inc',
    $include_dir . '/common.inc',
    $module_dir . '/entity/entity.module',
    $module_dir . '/entity/entity.controller.inc',
    $module_dir . '/system/system.module',
    $include_dir . '/database/query.inc',
    $include_dir . '/database/select.inc',
    $module_dir . '/entity/entity.module',
    $include_dir . '/registry.inc',
    $include_dir . '/module.inc',
    $include_dir . '/menu.inc',
    $include_dir . '/file.inc', 
  );

  foreach ($includes as $include) {
    if (file_exists($include)) {
      require_once($include);
    }
  }
  if (drush_drupal_major_version() < 7) {
    return drush_module_cache_rebuild();
  }

  // This section is not functionally important. It's just getting the
  // registry_parsed_files() so that it can report the change.
  $connection_info = Database::getConnectionInfo();
  $driver = $connection_info['default']['driver'];
  require_once $include_dir . '/database/' . $driver . '/query.inc';

  $parsed_before = registry_get_parsed_files();

  cache_clear_all('lookup_cache', 'cache_bootstrap');
  cache_clear_all('variables', 'cache_bootstrap');
  cache_clear_all('module_implements', 'cache_bootstrap');

  registry_rebuild();   // Call Drupal 7+ registry_rebuild()
  drush_log(dt('Registry has been rebuilt.'));

  drush_log(dt('Bootstrapping to DRUPAL_BOOTSTRAP_FULL.'));
  drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
  drush_log(dt('Doing registry_rebuild() in DRUPAL_BOOTSTRAP_FULL.'));

  if (!drush_get_option('no-cache-clear')) {
    db_query('TRUNCATE {cache}');
  }

  registry_rebuild();
  $parsed_after = registry_get_parsed_files();

  // Remove files which don't exist anymore.
  $filenames = array();
  foreach ($parsed_after as $filename => $file) {
    if (!file_exists($filename)) {
      $filenames[] = $filename;
    }
  }

  if (!empty($filenames)) {
    db_delete('registry_file')
      ->condition('filename', $filenames)
      ->execute();
    db_delete('registry')
      ->condition('filename', $filenames)
      ->execute();

    $dt_args = array(
      '@files' => implode(', ', $filenames),
    );
    $singular = 'Manually deleted 1 stale file from the registry.';
    $plural   = 'Manually deleted @count stale files from the registry.';
    drush_log(format_plural(count($filenames), $singular, $plural), 'success');
    $singular = "A file has been declared in a module's .info, but could not be found. This is probably indicative of a bug. The missing file is @files.";
    $plural   = "@count files were declared in a module's .info, but could not be found. This is probably indicative of a bug. The missing files are @files.";
    drush_log(format_plural(count($filenames), $singular, $plural, $dt_args), 'warning');
  }

  $parsed_after = registry_get_parsed_files();

  $message = 'There were @parsed_before files in the registry before and @parsed_after files now.';
  $dt_args = array(
    '@parsed_before' => count($parsed_before),
    '@parsed_after'  => count($parsed_after),
  );
  drush_log(dt($message, $dt_args));

  drush_log(dt('The registry has been rebuilt.'), 'success');

  if (!drush_get_option('no-cache-clear')) {
    drush_drupal_cache_clear_all();
  }
  else {
    drush_log(dt('The caches have not been cleared. It is recommended you clear the Drupal caches as soon as possible.'), 'warning');
  }
}

/**
 * Rebuild for D6 (which really just means rebuilding the menu/system table).
 */
function drush_module_cache_rebuild() {
  drush_log(dt('menu_rebuild() in DRUSH_BOOTSTRAP_DRUPAL_DATABASE.'));
  module_list(TRUE, FALSE);
  module_rebuild_cache();
  drush_log(dt('Module cache has been rebuilt.'));
  drush_log(dt('Bootstrapping to DRUPAL_BOOTSTRAP_FULL.'));
  drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
  if (!drush_get_option('no-cache-clear')) {
    drush_drupal_cache_clear_all();
  }
  else {
    drush_log(dt('The caches have not been cleared. It is recommended you clear the Drupal caches as soon as possible.'), 'warning');
  }
}
