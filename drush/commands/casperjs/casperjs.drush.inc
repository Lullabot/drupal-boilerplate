<?php

/**
 * @file
 *
 * CasperJS Drush integration.
 */

/**
 * Implements hook_drush_command().
 */
function casperjs_drush_command() {
  $items['casperjs'] = array(
    'description' => 'Wrapper for running CasperJS tests. Accepts extra options for casperjs command.',
    'callback' => 'drush_casperjs',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_SITE,
    'arguments' => array(
      'tests' => 'A comma separated list of tests to run. If not provided, all tests at the tests directory will be run.',
    ),
    'options' => array(
      'test-root' => 'Overrides the default location of tests.',
      'url' => 'The base URL to use for test requests. Defaults to the URL set by Drush or the current Drush alias.',
      'includes' => 'Comma-separated list of files to include before each test file execution.',
      'cookies-file' => 'Sets the file name to store the persistent cookies. If not provided a random file in the system temporary directory will be used.',
    ),
    'examples' => array(
      'drush casperjs --url=http://boiler.local' => 'Runs all tests located at the tests directory against http://boiler.local',
      'drush casperjs --url=http://boiler.local homepage.js' => 'Runs homepage.js test against http://boiler.local.',
      'export BOILER_TEST_CAPTURE=true && drush -v casperjs --url=http://boiler.local --log-level=debug' => 'Runs all tests against http://boiler.local with extra verbose logging and taking screenshots on failed assertions.',
    ),
    'allow-additional-options' => TRUE,
  );

  return $items;
}

/**
 * Implements drush_COMMANDFILE().
 */
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

  // Add include files.
  $command .= ' --includes=' . drush_escapeshellarg(dirname(__FILE__) . '/includes/common.js,' . dirname(__FILE__) . '/includes/session.js');
  $command .= ' --pre=' . drush_escapeshellarg(dirname(__FILE__) . '/includes/pre-test.js');
  $command .= ' --post=' . drush_escapeshellarg(dirname(__FILE__) . '/includes/post-test.js');
  if ($includes = drush_get_option('includes')) {
    $command .= ',' . $includes;
  }

  // Set the root where tests are. Defaults to the tests directory
  if (!$root = drush_get_option('test-root')) {
    $root =  drush_get_context('DRUSH_DRUPAL_ROOT') . '/../tests';
  }

  // If a list of tests to run were passed, append them to the command.
  // Otherwise, just set the root where tests are located so CasperJS
  // will run all of them.
  $tests_to_run = $root;
  if ($tests) {
    $tests_to_run = '';
    foreach (explode(',', $tests) as $test_file) {
      $tests_to_run .= ' ' . drush_escapeshellarg($root . '/' . $test_file);
    }
  }
  $command .= ' ' . $tests_to_run;

  // Append additional CasperJS options to the command.
  $args = array();
  foreach (drush_get_original_cli_args_and_options() as $arg) {
    // Don't pass some options through.
    if (strpos($arg, '--test-root') !== FALSE
      || strpos($arg, '--simulate') !== FALSE
      || strpos($arg, '--includes') !== FALSE
      || strpos($arg, '-') !== 0) {
      continue;
    }
    $args[] = $arg;
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
