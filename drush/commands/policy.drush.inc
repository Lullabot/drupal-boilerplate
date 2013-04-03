<?php
/**
 * @file
 *   Policy commandfile.
 *
 *   Validates commands as they are issued and returns an error
 *   or changes options when policy is violated.
 */

/**
 * Implements drush_hook_COMMAND_validate().
 *
 * Prevent catastrophic braino when syncing databases.
 */
function drush_policy_sql_sync_validate($source = NULL, $destination = NULL) {
  if (preg_match('/live/', $destination)) {
    return drush_set_error(dt('You may never overwrite the production database.'));
  }
}

/**
 * Implements drush_hook_COMMAND_validate().
 *
 * Prevent catastrophic braino when rsyncing files.
 */
function drush_policy_core_rsync_validate($source = NULL, $destination = NULL) {
  if (preg_match('/live/', $destination)) {
    return drush_set_error(dt('You may never overwrite the production files.'));
  }
}
