<?php

/**
 * Implements hook_drush_command().
 */
function casperjs_drush_command() {
  $items['casperjs'] = array(
    'description' => 'Run casperjs tests.',
    'callback' => 'drush_casperjs',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_SITE,
    'arguments' => array(
      'tests' => 'A list of tests to run separated by a space. If not provided, all tests will be run.',
    ),
    'options' => array(
      'test-root' => 'The directory that contains the test files.',
      'url' => 'The base URL to use for test requests. Defaults to the URL set by Drush or the current Drush alias.',
      'includes' => 'Comma-separated list of files to include before each test file execution.',
      'cookies-file' => 'Sets the file name to store the persistent cookies. If not provided a random file in the system temporary directory will be used.',
    ),
    'allow-additional-options' => TRUE,
  );

  return $items;
}

function drush_casperjs($tests = NULL) {

  $command = 'casperjs test --verbose';

  if (!drush_get_option('cookies-file')) {
    $cookie_file = drush_tempnam('casper-cookie-');
    $command .= ' --cookies-file=' . drush_escapeshellarg($cookie_file);
  }

  if (!drush_get_option('url')) {
    $uri = drush_get_context('DRUSH_SELECTED_URI');
    $command .= ' --url=' . drush_escapeshellarg($uri);
  }

  $command .= ' --includes=' . drush_escapeshellarg(dirname(__FILE__) . '/common.js');
  if ($includes = drush_get_option('includes')) {
    $command .= ',' . $includes;
  }

  if ($root = drush_get_option('test-root')) {
    if (!empty($tests)) {
      // @todo Ensure that test-root option doesn't get passed through.
      $command .= ' --root=' . drush_escapeshellarg($root);
    }
    else {
      // If no specific test files have been passed in, then instead of using
      // --root we should just pass in this directory as an argument so that
      // casperjs can find all tests files itself.
      $command .= ' ' . drush_escapeshellarg($root);
    }
  }

  $args = array();
  foreach (drush_get_original_cli_args_and_options() as $arg) {
    // Don't pass some options through.
    if (strpos($arg, '--test-root') !== FALSE
      || strpos($arg, '--simulate') !== FALSE
      || strpos($arg, '--includes') !== FALSE) {
      continue;
    }
    $args[] = drush_escapeshellarg($arg);
  }
  if (!empty($args)) {
    $command .= ' ' . implode(' ', $args);
  }

  echo $command . "\n";
  $result = drush_shell_proc_open($command);
  if ($result !== 0) {
    return drush_set_error('CASPERJS_TEST_FAILED', dt('Tests failed.'));
  }
  else {
    drush_log(dt('Tests succeeded.'), 'success');
    return TRUE;
  }
}
