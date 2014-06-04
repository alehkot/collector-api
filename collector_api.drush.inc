<?php

/**
 * Implements hook_drush_command().
 */
function collector_api_drush_command() {
  $items = array();
  $options = array();

  $items['collector_api_export'] = array(
    'description' => dt('Return items in the collector.'),
    'aliases' => array('cae'),
    'options' => $options,
    'outputformat' => array(
      'default' => 'table',
      'pipe-format' => 'list',
      'field-labels' => array(
        'cid' => 'Collector ID',
        'uid' => 'User ID',
        'session_id' => 'Session ID',
        'path' => 'Path',
        'created' => 'Created',
      ),
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
      foreach ($data as $data_key => $data_value) {
       $result[$cid]["data-${data_key}"] = $data_value;
      }
    }
  }

  return $result;
}
