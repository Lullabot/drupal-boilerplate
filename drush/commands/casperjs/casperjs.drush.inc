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
      'tests' => 'A comma separated list of tests to run. If not provided, all tests will be run.',
    ),
    'allow-additional-options' => TRUE,
  );

  return $items;
}

function drush_casperjs($tests = NULL) {
  $cookie_file = drush_tempnam('casper-cookie-');
  $uri = drush_get_context('DRUSH_SELECTED_URI');

  $command = 'capserjs test';
  $command .= ' --cookies-file=' . drush_escapeshellarg($cookie_file);
  $command .= ' --url=' . drush_escapeshellarg($uri);
  $command .= ' --includes=' . drush_escapeshellarg(dirname(__FILE__) . '/common.js');

  if ($root = drush_get_context('DRUSH_REPO_ROOT')) {
    $command .= ' --root=' . drush_escapeshellarg($root);
  }
  else {
    $root = drush_get_option('root') ? drush_get_option('root') : getcwd();
    $command .= ' --root=' . drush_escapeshellarg($root);
  }

  if ($tests) {
    foreach (explode(',', $tests) as $test) {
      $command .= ' ' . drush_escapeshellarg('tests/' . $test);
    }
  }

  $args = drush_get_original_cli_args_and_options();
  if (!empty($tests)) {
    array_shift($args);
  }
  for ($x = 0; $x < count($args); $x++) {
    $args[$x] = drush_escapeshellarg($args[$x]);
  }
  $command .= ' ' . implode(' ', $args);

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
