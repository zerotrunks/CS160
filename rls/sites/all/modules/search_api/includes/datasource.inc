<?php

/**
 * @file
 * Contains the SearchApiDataSourceControllerInterface as well as a default base class.
 */

/**
 * Interface for all data source controllers for Search API indexes.
 *
 * Data source controllers encapsulate all operations specific to an item type.
 * They are used for loading items, extracting item data, keeping track of the
 * item status, etc.
 *
 * All methods of the data source may throw exceptions of type
 * SearchApiDataSourceException if any exception or error state is encountered.
 */
interface SearchApiDataSourceControllerInterface {

  /**
   * Constructor for a data source controller.
   *
   * @param $type
   *   The item type for which this controller is created.
   */
  public function __construct($type);

  /**
   * Return information on the ID field for this controller's type.
   *
   * @return array
   *   An associative array containing the following keys:
   *   - key: The property key for the ID field, as used in the item wrapper.
   *   - type: The type of the ID field. Has to be one of the types from
   *     search_api_field_types(). List types ("list<*>") are not allowed.
   */
  public function getIdFieldInfo();

  /**
   * Load items of the type of this data source controller.
   *
   * @param array $ids
   *   The IDs of the items to laod.
   *
   * @return array
   *   The loaded items, keyed by ID.
   */
  public function loadItems(array $ids);

  /**
   * Get a metadata wrapper for the item type of this data source controller.
   *
   * @param $item
   *   Unless NULL, an item of the item type for this controller to be wrapped.
   * @param array $info
   *   Optionally, additional information that should be used for creating the
   *   wrapper. Uses the same format as entity_metadata_wrapper().
   *
   * @return EntityMetadataWrapper
   *   A wrapper for the item type of this data source controller, according to
   *   the info array, and optionally loaded with the given data.
   *
   * @see entity_metadata_wrapper()
   */
  public function getMetadataWrapper($item = NULL, array $info = array());

  /**
   * Get the unique ID of an item.
   *
   * @param $item
   *   An item of this controller's type.
   *
   * @return
   *   Either the unique ID of the item, or NULL if none is available.
   */
  public function getItemId($item);

  /**
   * Get a human-readable label for an item.
   *
   * @param $item
   *   An item of this controller's type.
   *
   * @return
   *   Either a human-readable label for the item, or NULL if none is available.
   */
  public function getItemLabel($item);

  /**
   * Get a URL at which the item can be viewed on the web.
   *
   * @param $item
   *   An item of this controller's type.
   *
   * @return
   *   Either an array containing the 'path' and 'options' keys used to build
   *   the URL of the item, and matching the signature of url(), or NULL if the
   *   item has no URL of its own.
   */
  public function getItemUrl($item);

  /**
   * Initialize tracking of the index status of items for the given indexes.
   *
   * All currently known items of this data source's type should be inserted
   * into the tracking table for the given indexes, with status "changed". If
   * items were already present, these should also be set to "changed" and not
   * be inserted again.
   *
   * @param array $indexes
   *   The SearchApiIndex objects for which item tracking should be initialized.
   *
   * @throws SearchApiDataSourceException
   *   If any of the indexes doesn't use the same item type as this controller.
   */
  public function startTracking(array $indexes);

  /**
   * Stop tracking of the index status of items for the given indexes.
   *
   * The tracking tables of the given indexes should be completely cleared.
   *
   * @param array $indexes
   *   The SearchApiIndex objects for which item tracking should be stopped.
   *
   * @throws SearchApiDataSourceException
   *   If any of the indexes doesn't use the same item type as this controller.
   */
  public function stopTracking(array $indexes);

  /**
   * Start tracking the index status for the given items on the given indexes.
   *
   * @param array $item_ids
   *   The IDs of new items to track.
   * @param array $indexes
   *   The indexes for which items should be tracked.
   *
   * @throws SearchApiDataSourceException
   *   If any of the indexes doesn't use the same item type as this controller.
   */
  public function trackItemInsert(array $item_ids, array $indexes);

  /**
   * Set the tracking status of the given items to "changed"/"dirty".
   *
   * Unless $dequeue is set to TRUE, this operation is ignored for items whose
   * status is not "indexed".
   *
   * @param $item_ids
   *   Either an array with the IDs of the changed items. Or FALSE to mark all
   *   items as changed for the given indexes.
   * @param array $indexes
   *   The indexes for which the change should be tracked.
   * @param $dequeue
   *   If set to TRUE, also change the status of queued items.
   *
   * @throws SearchApiDataSourceException
   *   If any of the indexes doesn't use the same item type as this controller.
   */
  public function trackItemChange($item_ids, array $indexes, $dequeue = FALSE);

  /**
   * Set the tracking status of the given items to "queued".
   *
   * Queued items are not marked as "dirty" even when they are changed, and they
   * are not returned by the getChangedItems() method.
   *
   * @param $item_ids
   *   Either an array with the IDs of the queued items. Or FALSE to mark all
   *   items as queued for the given indexes.
   * @param SearchApiIndex $index
   *   The index for which the items were queued.
   *
   * @throws SearchApiDataSourceException
   *   If any of the indexes doesn't use the same item type as this controller.
   */
  public function trackItemQueued($item_ids, SearchApiIndex $index);

  /**
   * Set the tracking status of the given items to "indexed".
   *
   * @param array $item_ids
   *   The IDs of the indexed items.
   * @param SearchApiIndex $indexes
   *   The index on which the items were indexed.
   *
   * @throws SearchApiDataSourceException
   *   If the index doesn't use the same item type as this controller.
   */
  public function trackItemIndexed(array $item_ids, SearchApiIndex $index);

  /**
   * Stop tracking the index status for the given items on the given indexes.
   *
   * @param array $item_ids
   *   The IDs of the removed items.
   * @param array $indexes
   *   The indexes for which the deletions should be tracked.
   *
   * @throws SearchApiDataSourceException
   *   If any of the indexes doesn't use the same item type as this controller.
   */
  public function trackItemDelete(array $item_ids, array $indexes);

  /**
   * Get a list of items that need to be indexed.
   *
   * If possible, completely unindexed items should be returned before items
   * that were indexed but later changed. Also, items that were changed longer
   * ago should be favored.
   *
   * @param SearchApiIndex $index
   *   The index for which changed items should be returned.
   * @param $limit
   *   The maximum number of items to return. Negative values mean "unlimited".
   *
   * @return array
   *   The IDs of items that need to be indexed for the given index.
   */
  public function getChangedItems(SearchApiIndex $index, $limit = -1);

  /**
   * Get information on how many items have been indexed for a certain index.
   *
   * @param SearchApiIndex $index
   *   The index whose index status should be returned.
   *
   * @return array
   *   An associative array containing two keys (in this order):
   *   - indexed: The number of items already indexed in their latest version.
   *   - total: The total number of items that have to be indexed for this
   *     index.
   *
   * @throws SearchApiDataSourceException
   *   If the index doesn't use the same item type as this controller.
   */
  public function getIndexStatus(SearchApiIndex $index);

}

/**
 * Default base class for the SearchApiDataSourceControllerInterface.
 *
 * Contains default implementations for a number of methods which will be
 * similar for most data sources. Concrete data sources can decide to extend
 * this base class to save time, but can also implement the interface directly.
 *
 * A subclass will still have to provide implementations for the following
 * methods:
 * - getIdFieldInfo()
 * - loadItems()
 * - getMetadataWrapper() or getPropertyInfo()
 * - startTracking() or getAllItemIds()
 *
 * The table used by default for tracking the index status of items is
 * {search_api_item}. This can easily be changed, for example when an item type
 * has non-integer IDs, by changing the $table property.
 */
abstract class SearchApiAbstractDataSourceController implements SearchApiDataSourceControllerInterface {

  /**
   * The item type for this controller instance.
   */
  protected $type;

  /**
   * The info array for the item type, as specified via
   * hook_search_api_item_type_info().
   *
   * @var array
   */
  protected $info;

  /**
   * The table used for tracking items. Set to NULL on subclasses to disable
   * the default tracking for an item type, or change the property to use a
   * different table for tracking.
   *
   * @var string
   */
  protected $table = 'search_api_item';

  /**
   * When using the default tracking mechanism: the name of the column on
   * $this->table containing the item ID.
   *
   * @var string
   */
  protected $itemIdColumn = 'item_id';

  /**
   * When using the default tracking mechanism: the name of the column on
   * $this->table containing the index ID.
   *
   * @var string
   */
  protected $indexIdColumn = 'index_id';

  /**
   * When using the default tracking mechanism: the name of the column on
   * $this->table containing the indexing status.
   *
   * @var string
   */
  protected $changedColumn = 'changed';

  /**
   * Constructor for a data source controller.
   *
   * @param $type
   *   The item type for which this controller is created.
   */
  public function __construct($type) {
    $this->type = $type;
    $this->info = search_api_get_item_type_info($type);
  }

  /**
   * Get a metadata wrapper for the item type of this data source controller.
   *
   * @param $item
   *   Unless NULL, an item of the item type for this controller to be wrapped.
   * @param array $info
   *   Optionally, additional information that should be used for creating the
   *   wrapper. Uses the same format as entity_metadata_wrapper().
   *
   * @return EntityMetadataWrapper
   *   A wrapper for the item type of this data source controller, according to
   *   the info array, and optionally loaded with the given data.
   *
   * @see entity_metadata_wrapper()
   */
  public function getMetadataWrapper($item = NULL, array $info = array()) {
    $info += $this->getPropertyInfo();
    return entity_metadata_wrapper($this->type, $item, $info);
  }

  /**
   * Helper method that can be used by subclasses to specify the property
   * information to use when creating a metadata wrapper.
   *
   * @return array
   *   Property information as specified by hook_entity_property_info().
   *
   * @see hook_entity_property_info()
   */
  protected function getPropertyInfo() {
    throw new SearchApiDataSourceException(t('No known property information for type !type.', array('!type' => $this->type)));
  }

  /**
   * Get the unique ID of an item.
   *
   * @param $item
   *   An item of this controller's type.
   *
   * @return
   *   Either the unique ID of the item, or NULL if none is available.
   */
  public function getItemId($item) {
    $id_info = $this->getIdFieldInfo();
    $field = $id_info['key'];
    $wrapper = $this->getMetadataWrapper($item);
    if (!isset($wrapper->$field)) {
      return NULL;
    }
    $id = $wrapper->$field->value();
    return $id ? $id : NULL;
  }

  /**
   * Get a human-readable label for an item.
   *
   * @param $item
   *   An item of this controller's type.
   *
   * @return
   *   Either a human-readable label for the item, or NULL if none is available.
   */
  public function getItemLabel($item) {
    $label = $this->getMetadataWrapper($item)->label();
    return $label ? $label : NULL;
  }

  /**
   * Get a URL at which the item can be viewed on the web.
   *
   * @param $item
   *   An item of this controller's type.
   *
   * @return
   *   Either an array containing the 'path' and 'options' keys used to build
   *   the URL of the item, and matching the signature of url(), or NULL if the
   *   item has no URL of its own.
   */
  public function getItemUrl($item) {
    return NULL;
  }

  /**
   * Initialize tracking of the index status of items for the given indexes.
   *
   * All currently known items of this data source's type should be inserted
   * into the tracking table for the given indexes, with status "changed". If
   * items were already present, these should also be set to "changed" and not
   * be inserted again.
   *
   * @param array $indexes
   *   The SearchApiIndex objects for which item tracking should be initialized.
   *
   * @throws SearchApiDataSourceException
   *   If any of the indexes doesn't use the same item type as this controller.
   */
  public function startTracking(array $indexes) {
    if (!$this->table) {
      return;
    }
    // We first clear the tracking table for all indexes, so we can just insert
    // all items again without any key conflicts.
    $this->stopTracking($indexes);
    // Insert all items as new.
    $this->trackItemInsert($this->getAllItemIds(), $indexes);
  }

  /**
   * Helper method that can be used by subclasses instead of implementing startTracking().
   *
   * Returns the IDs of all items that are known for this controller's type.
   *
   * @return array
   *   An array containing all item IDs for this type.
   */
  protected function getAllItemIds() {
    throw new SearchApiDataSourceException(t('Items not known for type !type.', array('!type' => $this->type)));
  }

  /**
   * Stop tracking of the index status of items for the given indexes.
   *
   * The tracking tables of the given indexes should be completely cleared.
   *
   * @param array $indexes
   *   The SearchApiIndex objects for which item tracking should be stopped.
   *
   * @throws SearchApiDataSourceException
   *   If any of the indexes doesn't use the same item type as this controller.
   */
  public function stopTracking(array $indexes) {
    if (!$this->table) {
      return;
    }
    // We could also use a single query with "IN" operator, but this method
    // will mostly be called with only one index.
    foreach ($indexes as $index) {
      $this->checkIndex($index);
      $query = db_delete($this->table)
        ->condition($this->indexIdColumn, $index->id)
        ->execute();
    }
  }

  /**
   * Start tracking the index status for the given items on the given indexes.
   *
   * @param array $item_ids
   *   The IDs of new items to track.
   * @param array $indexes
   *   The indexes for which items should be tracked.
   *
   * @throws SearchApiDataSourceException
   *   If any of the indexes doesn't use the same item type as this controller.
   */
  public function trackItemInsert(array $item_ids, array $indexes) {
    if (!$this->table) {
      return;
    }
    $insert = db_insert($this->table)
      ->fields(array($this->itemIdColumn, $this->indexIdColumn, $this->changedColumn));
    foreach ($item_ids as $item_id) {
      foreach ($indexes as $index) {
        $this->checkIndex($index);
        $insert->values(array(
          $this->itemIdColumn => $item_id,
          $this->indexIdColumn => $index->id,
          $this->changedColumn => 1,
        ));
      }
    }
    $insert->execute();
  }

  /**
   * Set the tracking status of the given items to "changed"/"dirty".
   *
   * Unless $dequeue is set to TRUE, this operation is ignored for items whose
   * status is not "indexed".
   *
   * @param $item_ids
   *   Either an array with the IDs of the changed items. Or FALSE to mark all
   *   items as changed for the given indexes.
   * @param array $indexes
   *   The indexes for which the change should be tracked.
   * @param $dequeue
   *   If set to TRUE, also change the status of queued items.
   *
   * @throws SearchApiDataSourceException
   *   If any of the indexes doesn't use the same item type as this controller.
   */
  public function trackItemChange($item_ids, array $indexes, $dequeue = FALSE) {
    if (!$this->table) {
      return;
    }
    $index_ids = array();
    foreach ($indexes as $index) {
      $this->checkIndex($index);
      $index_ids[] = $index->id;
    }
    $update = db_update($this->table)
      ->fields(array(
        $this->changedColumn => REQUEST_TIME,
      ))
      ->condition($this->indexIdColumn, $index_ids, 'IN')
      ->condition($this->changedColumn, 0, $dequeue ? '<=' : '=');
    if ($item_ids !== FALSE) {
      $update->condition($this->itemIdColumn, $item_ids, 'IN');
    }
    $update->execute();
  }

  /**
   * Set the tracking status of the given items to "queued".
   *
   * Queued items are not marked as "dirty" even when they are changed, and they
   * are not returned by the getChangedItems() method.
   *
   * @param $item_ids
   *   Either an array with the IDs of the queued items. Or FALSE to mark all
   *   items as queued for the given indexes.
   * @param SearchApiIndex $index
   *   The index for which the items were queued.
   *
   * @throws SearchApiDataSourceException
   *   If any of the indexes doesn't use the same item type as this controller.
   */
  public function trackItemQueued($item_ids, SearchApiIndex $index) {
    if (!$this->table) {
      return;
    }
    $update = db_update($this->table)
      ->fields(array(
        $this->changedColumn => -1,
      ))
      ->condition($this->indexIdColumn, $index->id);
    if ($item_ids !== FALSE) {
      $update->condition($this->itemIdColumn, $item_ids, 'IN');
    }
    $update->execute();
  }

  /**
   * Set the tracking status of the given items to "indexed".
   *
   * @param array $item_ids
   *   The IDs of the indexed items.
   * @param SearchApiIndex $indexes
   *   The index on which the items were indexed.
   *
   * @throws SearchApiDataSourceException
   *   If the index doesn't use the same item type as this controller.
   */
  public function trackItemIndexed(array $item_ids, SearchApiIndex $index) {
    if (!$this->table) {
      return;
    }
    $this->checkIndex($index);
    db_update($this->table)
      ->fields(array(
        $this->changedColumn => 0,
      ))
      ->condition($this->itemIdColumn, $item_ids, 'IN')
      ->condition($this->indexIdColumn, $index->id)
      ->execute();
  }

  /**
   * Stop tracking the index status for the given items on the given indexes.
   *
   * @param array $item_ids
   *   The IDs of the removed items.
   * @param array $indexes
   *   The indexes for which the deletions should be tracked.
   *
   * @throws SearchApiDataSourceException
   *   If any of the indexes doesn't use the same item type as this controller.
   */
  public function trackItemDelete(array $item_ids, array $indexes) {
    if (!$this->table) {
      return;
    }
    $index_ids = array();
    foreach ($indexes as $index) {
      $this->checkIndex($index);
      $index_ids[] = $index->id;
    }
    db_delete($this->table)
      ->condition($this->itemIdColumn, $item_ids, 'IN')
      ->condition($this->indexIdColumn, $index_ids, 'IN')
      ->execute();
  }

  /**
   * Get a list of items that need to be indexed.
   *
   * If possible, completely unindexed items should be returned before items
   * that were indexed but later changed. Also, items that were changed longer
   * ago should be favored.
   *
   * @param SearchApiIndex $index
   *   The index for which changed items should be returned.
   * @param $limit
   *   The maximum number of items to return. Negative values mean "unlimited".
   *
   * @return array
   *   The IDs of items that need to be indexed for the given index.
   */
  public function getChangedItems(SearchApiIndex $index, $limit = -1) {
    if ($limit == 0) {
      return array();
    }
    $this->checkIndex($index);
    $select = db_select($this->table, 't');
    $select->addField('t', 'item_id');
    $select->condition($this->indexIdColumn, $index->id);
    $select->condition($this->changedColumn, 0, '>');
    $select->orderBy($this->changedColumn, 'ASC');
    if ($limit > 0) {
      $select->range(0, $limit);
    }
    return $select->execute()->fetchCol();
  }

  /**
   * Get information on how many items have been indexed for a certain index.
   *
   * @param SearchApiIndex $index
   *   The index whose index status should be returned.
   *
   * @return array
   *   An associative array containing two keys (in this order):
   *   - indexed: The number of items already indexed in their latest version.
   *   - total: The total number of items that have to be indexed for this
   *     index.
   */
  public function getIndexStatus(SearchApiIndex $index) {
    if (!$this->table) {
      return array('indexed' => 0, 'total' => 0);
    }
    $this->checkIndex($index);
    $indexed = db_select($this->table, 'i')
      ->condition($this->indexIdColumn, $index->id)
      ->condition($this->changedColumn, 0)
      ->countQuery()
      ->execute()
      ->fetchField();
    $total = db_select($this->table, 'i')
      ->condition($this->indexIdColumn, $index->id)
      ->countQuery()
      ->execute()
      ->fetchField();
    return array('indexed' => $indexed, 'total' => $total);
  }

  /**
   * Helper method for ensuring that an index uses the same item type as this controller.
   *
   * @param SearchApiIndex $index
   *   The index to check.
   *
   * @throws SearchApiDataSourceException
   *   If the index doesn't use the same type as this controller.
   */
  protected function checkIndex(SearchApiIndex $index) {
    if ($index->item_type != $this->type) {
      $index_type = search_api_get_item_type_info($index->item_type);
      $index_type = empty($index_type['name']) ? $index->item_type : $index_type['name'];
      $msg = t('Invalid index !index of type !index_type passed to data source controller for type !this_type.',
          array('!index' => $index->name, '!index_type' => $index_type, '!this_type' => $this->info['name']));
      throw new SearchApiDataSourceException($msg);
    }
  }

}
