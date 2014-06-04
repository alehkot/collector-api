<?php

/**
 * Implements hook_drush_command().
 */
function collector_api_drush_command() {
  $items = array();
  $options = array();

  $providers_data = array(
    'title' => array(),
    'hostname' => array(),
    'timer' => array(),
  );
  drupal_alter('collector_api_data', $providers_data);

  $field_labels = array(
    'cid' => 'Collector ID',
    'uid' => 'User ID',
    'session_id' => 'Session ID',
    'path' => 'Path',
    'created' => 'Created',
  );

  $providers_keys = array_keys($providers_data);
  foreach ($providers_keys as $key) {
    $key_ = _collector_api_generate_data_key($key);
    $field_labels["data_${key_}"] = ucfirst($key);
  }

  $items['collector_api_export'] = array(
    'description' => dt('Return items in the collector.'),
    'aliases' => array('cae'),
    'options' => $options,
    'outputformat' => array(
      'default' => 'table',
      'pipe-format' => 'list',
      'field-labels' => $field_labels,
      'output-data-type' => 'format-table',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function collector_api_drush_help($section) {
  switch ($section) {
    case 'drush:collector_api_export':
      return dt('Return items in the collector.');
      break;
  }
}

/**
 * Returns items in the collector.
 */
function drush_collector_api_export() {
  $result = array();
  $collector_items = entity_load('collector_item');
  foreach ($collector_items as $item) {
    $item = (array)$item;
    $cid = $item['cid'];
    unset($item['cid']);

    // Flatten the data.
    $data = unserialize($item['data']);
    unset($item['data']);

    // Build results.
    foreach ($item as $key => $value) {
      $result[$cid][$key] = $value;
    }
    if (!empty($data)) {
      foreach ($data as $data_key_unprocessed => $data_value) {
        $key_ = _collector_api_generate_data_key($data_key_unprocessed);
        $result[$cid]["data_${key_}"] = $data_value;
      }
    }
  }

  return $result;
}

/**
 *  Generates machine name from the provided key.
 */
function _collector_api_generate_data_key($key) {
  return preg_replace('/[^a-z0-9_]+/', '_', $key);
}
