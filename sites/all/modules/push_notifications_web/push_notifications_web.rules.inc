<?php

/**
 * @file
 * Emrys push notifications rules.
 */

/**
 * Implements hook_rules_action_info().
 */
function push_notifications_web_rules_action_info() {
  return array(
    'push_notifications_web_rules_action_send_notification_all' => array(
      'label' => t('Send PN to all'),
      'group' => t('Push Notifications'),
      'parameter' => array(
        'title' => array(
          'type' => 'text',
          'label' => t('Title of Push Notification'),
          'description' => t('Use replacement patterns for node and user data.'),
        ),
        'body' => array(
          'type' => 'text',
          'label' => t('Body of Push Notification'),
          'description' => t('Max length 200 symbols. String will be trimmed to 200 auto'),
        ),
        'link' => array(
          'type' => 'uri',
          'label' => t('Link for Push Notification onclick event'),
          'description' => t('Auto UTM adds from your SOURCE'),
        ),
        'icon' => array(
          'type' => 'uri',
          'label' => t('Icon for Push Notification'),
          'description' => t('Set custom absolute path or choose token'),
        ),
      ),
    ),
    'push_notifications_web_rules_action_send_notification_user' => array(
      'label' => t('Send PN to single user'),
      'group' => t('Push Notifications'),
      'parameter' => array(
        'title' => array(
          'type' => 'text',
          'label' => t('Title of Push Notification'),
          'description' => t('Use replacement patterns for node and user data.'),
        ),
        'body' => array(
          'type' => 'text',
          'label' => t('Body of Push Notification'),
          'description' => t('Max length 200 symbols. String will be trimmed to 200 auto'),
        ),
        'link' => array(
          'type' => 'uri',
          'label' => t('Link for Push Notification onclick event'),
          'description' => t('Auto UTM adds from your SOURCE'),
        ),
        'icon' => array(
          'type' => 'uri',
          'label' => t('Icon for Push Notification'),
          'description' => t('Set custom absolute path or choose token'),
        ),
        'account' => array(
          'type' => 'user',
          'label' => t('User to Sent Notification to'),
        ),
      ),
    ),
    'push_notifications_web_rules_action_send_notification_user_sub_id' => array(
      'label' => t('Send PN to single subscribtion ID'),
      'group' => t('Push Notifications'),
      'parameter' => array(
        'title' => array(
          'type' => 'text',
          'label' => t('Title of Push Notification'),
          'description' => t('Use replacement patterns for node and user data.'),
        ),
        'body' => array(
          'type' => 'text',
          'label' => t('Body of Push Notification'),
          'description' => t('Max length 200 symbols. String will be trimmed to 200 auto'),
        ),
        'link' => array(
          'type' => 'uri',
          'label' => t('Link for Push Notification onclick event'),
          'description' => t('Auto UTM adds from your SOURCE'),
        ),
        'icon' => array(
          'type' => 'uri',
          'label' => t('Icon for Push Notification'),
          'description' => t('Set custom absolute path or choose token'),
        ),
        'subid' => array(
          'type' => 'text',
          'label' => t('Subscribtion ID to Send Notification to'),
          'description' => t('Sometimes you can need to use Push for Anonymous users, with no user->uid. So you can get there subscription id stored in cookies of you site (lookfor "emdn_gcm_endpoint"). Get this endpoint and cut this part "https://android.googleapis.com/gcm/send/". So, everything left is subscriber ID. Store this ID in database (i.e. as field of node) and use for this field'),
        ),
      ),
    ),
    'push_notifications_web_rules_action_send_notification_endpoint' => array(
      'label' => t('Send PN to single endpoint'),
      'group' => t('Push Notifications'),
      'parameter' => array(
        'title' => array(
          'type' => 'text',
          'label' => t('Title of Push Notification'),
          'description' => t('Use replacement patterns for node and user data.'),
        ),
        'body' => array(
          'type' => 'text',
          'label' => t('Body of Push Notification'),
          'description' => t('Max length 200 symbols. String will be trimmed to 200 auto'),
        ),
        'link' => array(
          'type' => 'uri',
          'label' => t('Link for Push Notification onclick event'),
          'description' => t('Auto UTM adds from your SOURCE'),
        ),
        'icon' => array(
          'type' => 'uri',
          'label' => t('Icon for Push Notification'),
          'description' => t('Set custom absolute path or choose token'),
        ),
        'endpoint' => array(
          'type' => 'text',
          'label' => t('Endpoint to Send Notification to'),
          'description' => t('Sometimes you can need to use Push for Anonymous users, with no user->uid. So you can get there endpoint stored in cookies of you site (lookfor "emdn_gcm_endpoint"). Get this endpoint store in database (i.e. as field of node) and use for this field'),
        ),
      ),
    ),
  );
}

/**
 * Implements rule send notification all().
 */
function push_notifications_web_rules_action_send_notification_all($title, $body, $link, $icon) {
  $settings = push_notifications_web_get_settings();
  $pushem_uid = $settings['pushem_uid'];
  $pushem_key = $settings['pushem_key'];
  $pushem_source = $settings['pushem_source'];
  if ($pushem_uid != NULL && $pushem_key != NULL && $pushem_source != NULL) {
    $url = 'https://pushem.org/api/subscribers/get_all/?uid=' . $pushem_uid . '&key=' . $pushem_key . '&source=' . $pushem_source;
    $result = file_get_contents($url, FALSE);
    $result = json_decode($result);
    if (!empty($result)) {
      foreach ($result as $value) {
        $pushem_sub_ids[] = $value->id;
      }
      if (!empty($pushem_sub_ids)) {
        $pushem_sub_ids = implode(',', $pushem_sub_ids);
      }
      if (isset($pushem_sub_ids) && !empty($pushem_sub_ids)) {
        $time = time();
        $body = substr($body, 0, 200);
        $url = 'https://pushem.org/api/notifications/add/?uid=' . $pushem_uid . '&key=' . $pushem_key . '&source=' . $pushem_source . '&title=' . urlencode($title) . '&body=' . urlencode($body) . '&icon=' . urlencode($icon) . '&link=' . urlencode($link) . '&timestamp=' . $time . '&subscribers=' . $pushem_sub_ids;
        $result = file_get_contents($url, FALSE);
      }
    }
  }
}

/**
 * Implements rule send notification user().
 */
function push_notifications_web_rules_action_send_notification_user($title, $body, $link, $icon, $account) {
  $settings = push_notifications_web_get_settings();
  $pushem_uid = $settings['pushem_uid'];
  $pushem_key = $settings['pushem_key'];
  $pushem_source = $settings['pushem_source'];
  if ($pushem_uid != NULL && $pushem_key != NULL && $pushem_source != NULL) {
    $url = 'https://pushem.org/api/subscribers/get_by_site_id/?uid=' . $pushem_uid . '&key=' . $pushem_key . '&source=' . $pushem_source . '&site_id=' . $account->uid;
    $result = file_get_contents($url, FALSE);
    $result = json_decode($result);
    if (!empty($result)) {
      foreach ($result as $value) {
        $pushem_sub_ids[] = $value->id;
      }
      if (!empty($pushem_sub_ids)) {
        $pushem_sub_ids = implode(',', $pushem_sub_ids);
      }
      if (isset($pushem_sub_ids) && !empty($pushem_sub_ids)) {
        $time = time();
        $body = substr($body, 0, 200);
        $url = 'https://pushem.org/api/notifications/add/?uid=' . $pushem_uid . '&key=' . $pushem_key . '&source=' . $pushem_source . '&title=' . urlencode($title) . '&body=' . urlencode($body) . '&icon=' . urlencode($icon) . '&link=' . urlencode($link) . '&timestamp=' . $time . '&subscribers=' . $pushem_sub_ids;
        $result = file_get_contents($url, FALSE);
      }
    }
  }
}

/**
 * Implements rule send notification sub_id().
 */
function push_notifications_web_rules_action_send_notification_user_sub_id($title, $body, $link, $icon, $subid) {
  $settings = push_notifications_web_get_settings();
  $pushem_uid = $settings['pushem_uid'];
  $pushem_key = $settings['pushem_key'];
  $pushem_source = $settings['pushem_source'];
  if ($pushem_uid != NULL && $pushem_key != NULL && $pushem_source != NULL) {
    $url = 'https://pushem.org/api/subscribers/get_by_sub_id/?uid=' . $pushem_uid . '&key=' . $pushem_key . '&source=' . $pushem_source . '&sub_id=' . $subid;
    $result = file_get_contents($url, FALSE);
    $result = json_decode($result);
    if (!empty($result)) {
      foreach ($result as $value) {
        $pushem_sub_ids[] = $value->id;
      }
      if (!empty($pushem_sub_ids)) {
        $pushem_sub_ids = implode(',', $pushem_sub_ids);
      }
      if (isset($pushem_sub_ids) && !empty($pushem_sub_ids)) {
        $time = time();
        $body = substr($body, 0, 200);
        $url = 'https://pushem.org/api/notifications/add/?uid=' . $pushem_uid . '&key=' . $pushem_key . '&source=' . $pushem_source . '&title=' . urlencode($title) . '&body=' . urlencode($body) . '&icon=' . urlencode($icon) . '&link=' . urlencode($link) . '&timestamp=' . $time . '&subscribers=' . $pushem_sub_ids;
        $result = file_get_contents($url, FALSE);
      }
    }
  }
}

/**
 * Implements rule send notification endpoint().
 */
function push_notifications_web_rules_action_send_notification_endpoint($title, $body, $link, $icon, $endpoint) {
  $pushem_uid = variable_get('push_notifications_web_UID', NULL);
  $pushem_key = variable_get('push_notifications_web_API', NULL);
  $pushem_source = variable_get('push_notifications_web_SOURCE', NULL);
  $pushem_token = variable_get('push_notifications_web_TOKEN', NULL);
  if ($pushem_uid != NULL && $pushem_key != NULL && $pushem_source != NULL && $pushem_token != NULL) {
    $url = 'https://pushem.org/api/subscribers/get_by_endpoint/?token=' . $pushem_token . '&sub_endpoint=' . $endpoint;
    $result = file_get_contents($url, FALSE);
    $result = json_decode($result);
    if (!empty($result)) {
      foreach ($result as $value) {
        $pushem_sub_ids[] = $value->id;
      }
      if (!empty($pushem_sub_ids)) {
        $pushem_sub_ids = implode(',', $pushem_sub_ids);
      }
      if (isset($pushem_sub_ids) && !empty($pushem_sub_ids)) {
        $time = time();
        $body = substr($body, 0, 200);
        $url = 'https://pushem.org/api/notifications/add/?uid=' . $pushem_uid . '&key=' . $pushem_key . '&source=' . $pushem_source . '&title=' . urlencode($title) . '&body=' . urlencode($body) . '&icon=' . urlencode($icon) . '&link=' . urlencode($link) . '&timestamp=' . $time . '&subscribers=' . $pushem_sub_ids;
        $result = file_get_contents($url, FALSE);
      }
    }
  }
}
