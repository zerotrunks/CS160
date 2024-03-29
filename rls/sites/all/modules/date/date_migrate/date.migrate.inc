<?php

/**
 * @file
 * Support for migration into Date fields.
 */

class DateMigrateFieldHandler extends MigrateFieldHandler {
  public function __construct() {
    $this->registerTypes(array('date', 'datestamp', 'datetime'));
  }

  /*
   * Arguments for a date field migration.
   *
   * @param timezone
   *   Timezone (such as UTC, America/New_York, etc.) to apply.
   * @param timezone_db
   *   Timezone_db value for the field.
   * @param rrule
   *   Rule string for a repeating date field.
   * @param language
   *   Language of the text (defaults to destination language)
   */
  static function arguments($timezone = 'UTC', $timezone_db = 'UTC', $rrule = NULL, $language = NULL) {
    return get_defined_vars();
  }

  /**
   * Convert incoming data into the proper field arrays for Date fields.
   *
   * @param $entity
   *  The destination entity which will hold the field arrays.
   * @param array $field_info
   *  Metadata for the date field being populated.
   * @param array $instance
   *  Metadata for this instance of the date field being populated.
   * @param array $values
   *  Array of date values to be fielded.
   */
  public function prepare($entity, array $field_info, array $instance, array $values) {
    if (isset($values['arguments'])) {
      $arguments = $values['arguments'];
      unset($values['arguments']);
    }
    else {
      $arguments = array();
    }

    if (isset($arguments['timezone'])) {
      $default_timezone = $arguments['timezone'];
    }
    else {
      $default_timezone = 'UTC';
    }
    if (isset($arguments['timezone_db'])) {
      $default_timezone_db = $arguments['timezone_db'];
    }
    else {
      $default_timezone_db = NULL;
    }
    if (isset($arguments['rrule'])) {
      $default_rrule = $arguments['rrule'];
    }
    else {
      $default_rrule = NULL;
    }
    $language = $this->getFieldLanguage($entity, $field_info, $arguments);

    // Setup the standard Field API array for saving.
    $delta = 0;
    foreach ($values as $from) {
      // Set defaults
      $to = NULL;
      $timezone = $default_timezone;
      $timezone_db = $default_timezone_db;
      $rrule = $default_rrule;

      // Is the value a straight datetime value, or JSON containing a set of properties?
      if ($from{0} == '{') {
        $properties = drupal_json_decode($from);
        $from = $properties['from'];
        // Properties passed in with the date override any set via arguments
        if (!empty($properties['to'])) {
          $to = $properties['to'];
        }
        if (!empty($properties['timezone'])) {
          $timezone = $properties['timezone'];
        }
        if (!empty($properties['timezone_db'])) {
          $timezone_db = $properties['timezone_db'];
        }
        if (!empty($properties['rrule'])) {
          $rrule = $properties['rrule'];
        }
      }

      // Work from a timestamp
      $from = MigrationBase::timestamp($from);
      if ($to) {
        $to = MigrationBase::timestamp($to);
      }

      // What does the destination field expect?
      switch ($field_info['type']) {
        case 'datestamp':
          // Already done
          break;
        case 'datetime':
          // YYYY-MM-DD HH:MM:SS
          $from = format_date($from, 'custom', 'Y-m-d H:i:s', $timezone);
          if ($to) {
            $to = format_date($to, 'custom', 'Y-m-d H:i:s', $timezone);
          }
          break;
        case 'date':
          // ISO date: YYYY-MM-DDTHH:MM:SS
          $from = format_date($from, 'custom', 'Y-m-d\TH:i:s', $timezone);
          if ($to) {
            $to = format_date($to, 'custom', 'Y-m-d\TH:i:s', $timezone);
          }
          break;
        default:
          break;
      }

      // Handle repeats, coming in as RRULEs. Many field instances may be created.
      if ($instance['widget']['type'] == 'date_text_repeat' && $rrule) {
        include_once(DRUPAL_ROOT . '/' . drupal_get_path('module', 'date') . '/date_repeat.inc');
        include_once(DRUPAL_ROOT . '/' . drupal_get_path('module', 'date_api') . '/date_api_ical.inc');
        $item = array('value' => $from, 'value2' => $to, 'timezone' => $timezone);
        // Can be de-uglified when http://drupal.org/node/1159404 is committed.
        $return[$language] = date_repeat_build_dates(NULL, date_ical_parse_rrule($field_info, $rrule),
          $field_info, $item);
      }
      else {
        $return[$language][$delta]['value'] = $from;
        if (!empty($to)) {
          $return[$language][$delta]['value2'] = $to;
        }
      }
      $delta++;
    }
    if (!isset($return)) {
      $return = NULL;
    }
    return $return;
  }
}
