<?php

function yoursite_shiny_form_alter(&$form, &$form_state, $form_id) {
  if (!empty($form['#node_edit_form']) && $form['#node_edit_form'] === TRUE) {

    // Disable vertical tabs.
    $form['additional_settings']['#type'] = 'fieldset';
    $form['author']['#weight'] = -100;

    // Disable collapsible fieldsets.
    $disable_collapsible = array(
      'menu',
      'revision_information',
      'options'
    );
    foreach ($disable_collapsible as $element) {
      if (isset($form[$element]['#collapsible'])) {
        $form[$element]['#collapsible'] = FALSE;
      }
    }
  }

  // Change the order of the items in Additional Settings.
  $order = array(
    'options',
    'revision_information',
    'menu',
    'author',
  );

  for ($i = 0; $i < count($order); $i++) {
    if (!empty($form[$order[$i]])) {
      $form[$order[$i]]['#weight'] = $i;
    }
  }
}

/**
 * Implements hook_theme().
 */
function yoursite_shiny_theme($existing, $type, $theme, $path) {
  return array(
    'node_form' => array(
      'render element' => 'form',
      'template' => 'node-form',
      'path' => drupal_get_path('theme', 'yoursite_shiny') . '/templates',
    ),
  );
}

/**
 * Overrides template_preprocess_node_form
 */
function yoursite_shiny_preprocess_node_form(&$variables) {
  // Move to sidebar.
  $variables['additional_settings'] = $variables['form']['additional_settings'];
  hide($variables['form']['additional_settings']);

  $variables['buttons'] = $variables['form']['actions'];
  hide($variables['form']['actions']);
}
