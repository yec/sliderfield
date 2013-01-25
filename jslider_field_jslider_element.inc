<?php

/**
 * Implements hook_theme().
 */
function _jslider_field_theme_jslider() {
  $data['jslider_field_jslider'] = array(
    'render element' => 'element',
    'file' => 'jslider_field_jslider_element.inc',
  );
  return $data;
}

/**
 * Implements hook_element_info().
 */
function _jslider_field_element_info_jslider() {
  $types['jslider'] =  array(
    '#input'            => TRUE,
    '#process'          => array('jslider_field_jslider_process'),
    '#value_callback'   => 'jslider_field_jslider_value_callback',
    '#element_validate' => array('jslider_field_jslider_validate'),
    '#theme'            => array('jslider_field_jslider'),

    '#title' => NULL,
    '#input_title' => t('Min'),
    '#input2_title' => t('Max'),
    /**
     * Boolean: When set to true, the handle will animate with the default duration.
     * String: The name of a speed, such as "fast" or "slow".
     * Number: The duration of the animation, in milliseconds.
     */
    '#animate' => 'fast',
    /**
     * Disables the slider if set to true.
     */
    '#disabled' => FALSE,
    /**
     * The maximum value of the slider.
     */
    '#max' => 100,
    /**
     * The minimum value of the slider.
     */
    '#min' => 0,
    /**
     * Determines whether the slider handles move horizontally (min on left, max on right)
     * or vertically (min on bottom, max on top). Possible values: "horizontal", "vertical".
     */
    '#orientation' => 'horizontal',
    /**
     * Whether the slider represents a range.
     * Multiple types supported:
     *   Boolean: If set to true, the slider will detect if you have two handles and create a stylable range element between these two.
     *   String: Either "min" or "max". A min range goes from the slider min to one handle. A max range goes from one handle to the slider max.
     */
    '#range' => FALSE,
    /**
     * Determines the size or amount of each interval or step the slider takes between the min and max.
     * The full specified value range of the slider (max - min) should be evenly divisible by the step.
     */
    '#step' => 1,
    /**
     * Determines the value of the slider, if there's only one handle.
     * If there is more than one handle, determines the value of the first handle.
     */
    '#value' => 0,
    '#value2' => NULL, //When using range
    /**
     * Some default color styles for ease of use
     * red, green, blue
     */
    '#slider_style' => NULL,
    /**
     * Default size for input values
     */
    '#size' => 3,
    /**
     * If set to FALSE will display inputs only when javascript is disabled
     */
    '#display_inputs' => TRUE,
    /**
     * If enabled display the current values of slider
     * as simple text
     */
    '#display_values' => FALSE,
    /**
     * Acceptable types are the same as css with and height and it will be used as width
     * or height depending on #orientation
     */
    '#slider_length' => NULL,
    /**
     * Display the element inside a fieldset
     */
    '#display_inside_fieldset' => FALSE
  );
  return $types;
}

/**
 * Processes transfer slider: add textfields
 * @param unknown_type $element
 */
function jslider_field_jslider_process($element) {
  $element['#tree'] = TRUE;

  if ($element['#display_inside_fieldset']) {
    $element['slider'] = array(
      '#type'             => 'fieldset',
      '#title'    => $element['#title']
    );
  } elseif ($element['#title']) {
    $element['slider']['title'] = array(
      '#type'             => 'item',
      '#markup'    => '<label>' . $element['#title'] . '</label>'
    );
  }

  if ($element['#value'] < $element['#min']) {
    $element['#value'] = $element['#min'];
  }
  if ($element['#value'] > $element['#max']) {
    $element['#value'] = $element['#max'];
  }

  $values = array();
  $values[] = $element['#value'];

  // Generate input for slider
  $element['slider']['value'] = array(
    '#prefix'           => '<div id="' . $element['#id'] . '" class="jslider">',
    '#type'             => 'textfield',
    '#required'    => $element['#required'],
    '#title'             => $element['#input_title'],
    //'#value_callback'   => 'jslider_field_jslider_value_callback',
    '#default_value'    => $element['#value'],
    '#disabled'    => $element['#disabled'],
    '#element_validate' => array('jslider_field_validate_integer'),
    '#size'             => $element['#size'],
    '#attributes'       => array('class' => array('jslider-value-field')),
  );

  if (!is_null($element['#value2'])) {

    if ($element['#value2'] < $element['#min']) {
      $element['#value2'] = $element['#min'];
    }
    if ($element['#value2'] > $element['#max']) {
      $element['#value2'] = $element['#max'];
    }

    if ($element['#value2'] < $element['#value']) {
      $element['#value2'] = $element['#value'];
    }
    $values[] = $element['#value2'];
    $element['slider']['value2'] = array(
      '#type'             => 'textfield',
      '#default_value'    => $element['#value2'],
      '#required'    => $element['#required'],
      '#title'             => $element['#input2_title'],
      '#element_validate' => array('jslider_field_validate_integer'),
      '#disabled'    => $element['#disabled'],
      '#size'             => $element['#size'],
      '#attributes'       => array('class' => array('jslider-value2-field')),
    );
  }

  if ($element['#range'] === TRUE && is_null($element['#value2'])) {
    $element['#range'] = FALSE;
  }

  if ($element['#display_values']) {
    $element['slider']['values_text'] = array(
      '#type'       => 'item',
      //'#attributes' => array('class' => array('jslider-display-values-field')),
      '#markup'     => '<div class="jslider-display-values-field">' . implode(' - ', $values) . '</div>'
    );
  }

  $style = NULL;
  if (!is_null($element['#slider_length'])) {
    if ($element['#orientation'] == 'horizontal') {
      $style = "width : {$element['#slider_length']}";
    } else {
      $style = "height : {$element['#slider_length']}";
    }
  }

  // Create markup for slider container
  $element['slider']['container'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('jslider-container', $element['#slider_style']), 'style' => $style),
    '#attached' => array(
      'library' => array(
        array('system', 'ui.slider')
      ),
      'js' => array(
        drupal_get_path('module', 'jslider_field') . '/jslider_field_jslider.js',
        array(
          'data' => array(
            'jslider_field_' . $element['#id'] => array(
              'animate' => $element['#animate'],
              'disabled' => $element['#disabled'],
              'max' => $element['#max'] * 1,
              'min' => $element['#min'] * 1,
              'orientation' => $element['#orientation'],
              'range' => $element['#range'],
              'step' => $element['#step'] * 1,
              'display_inputs' => $element['#display_inputs']
            )
          ),
          'type' => 'setting',
        )
      ),
      'css' => array(
        drupal_get_path('module', 'jslider_field') . '/jslider_field_jslider.css',
      ),
    ),

    '#markup' => '',
    '#suffix'           => '</div>'
  );


  return $element;
}

/**
 * @param $element
 * @param bool $input
 * @param $form_state
 * @return mixed
 */
function jslider_field_jslider_value_callback($element, $input = FALSE, $form_state) {
  if ($input) {
    $form_state_value = $form_state['values'];
    $element['#parents'][] = 'slider';
    foreach ($element['#parents'] as $parent) {
      $form_state_value = isset($form_state_value[$parent]) ? $form_state_value[$parent] : 0;
    }
    $element_value = $form_state_value;
    return $element_value;
  }
}

/**
 * Validates jslider
 *
 * @param array $element
 * @param array $form_state
 */
function jslider_field_jslider_validate($element, &$form_state) {
  $form_state_value = $form_state['values'];
  $element['#parents'][] = 'slider';
  foreach ($element['#parents'] as $parent) {
    $form_state_value = isset($form_state_value[$parent]) ? $form_state_value[$parent] : 0;
  }
  $element_value = $form_state_value;
  if (@isset($form_state['values']['instance']['settings']['min'])) {
    $element['#max'] = $form_state['values']['instance']['settings']['max'];
    $element['#min'] = $form_state['values']['instance']['settings']['min'];
  }
  if ($element_value['value'] > $element['#max'] || $element_value['value'] < $element['#min']
    || (@$element_value['value2'] > $element['#max'] && @!is_null($element_value['value2'])) || (@$element_value['value2'] < $element['#min'] && @!is_null($element_value['value2']))) {
    form_error($element, t("The entered values are not within valid range."));
  }
}

/**
 * Theme output of jslider
 *
 * @param array $element
 */
function theme_jslider_field_jslider($vars) {
}


/**
 *
 */
function _jslider_field_field_widget_info_jslider() {
  return array(
    'jslider' => array(
      'label' => t('jSlider'),
      'field types' => array('number_integer'),
      'settings' => array(
        'jslider_settings' => array(
          /**
           * Boolean: When set to true, the handle will animate with the default duration.
           * String: The name of a speed, such as "fast" or "slow".
           * Number: The duration of the animation, in milliseconds.
           */
          'animate' => TRUE,
          /**
           * Determines whether the slider handles move horizontally (min on left, max on right)
           * or vertically (min on bottom, max on top). Possible values: "horizontal", "vertical".
           */
          'orientation' => 'horizontal',
          /**
           * Whether the slider represents a range.
           * Multiple types supported:
           *   Boolean: If set to true, the slider will detect if you have two handles and create a stylable range element between these two.
           *   String: Either "min" or "max". A min range goes from the slider min to one handle. A max range goes from one handle to the slider max.
           */
          'range' => FALSE,
          /**
           * Determines the size or amount of each interval or step the slider takes between the min and max.
           * The full specified value range of the slider (max - min) should be evenly divisible by the step.
           */
          'step' => 1,
          /**
           * Some default color styles for ease of use
           * red, green, blue
           */
          'slider_style' => NULL,
          /**
           * If enabled display the current values of slider
           * as simple text
           */
          'display_values' => TRUE,
          /**
           * Acceptable types are the same as css with and height and it will be used as width
           * or height depending on #orientation
           */
          'slider_length' => NULL,
          /**
           *
           */
          'hide_inputs' => TRUE,
        )
      ),
    ),
  );
}

/**
 * @param $field
 * @param $instance
 */
function _jslider_field_field_widget_settings_form_jslider($field, $instance) {
  $widget = $instance['widget'];
  $settings = $widget['settings']['jslider_settings'];

  if ($widget['type'] == 'jslider') {
    $form['jslider_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('jSlider Settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['jslider_settings']['animate'] = array(
      '#type' => 'select',
      '#title' => t('Animate'),
      '#options' => array(
        FALSE => t('Disable'),
        TRUE => t('Default'),
        'fast' => t('Fast'),
        'slow' => t('Slow'),
        'custom' => t('Custom')
      ),
      //'#description' => t('Determines whether the slider handles move horizontally (min on left, max on right) or vertically (min on bottom, max on top).'),
      '#default_value' => $settings['animate']
    );
    $form['jslider_settings']['orientation'] = array(
      '#type' => 'select',
      '#title' => t('Orientation'),
      '#options' => array(
        'horizontal' => t('Horizontal'),
        'vertical' => t('Vertical')
      ),
      '#require' => TRUE,
      '#description' => t('Determines whether the slider handles move horizontally (min on left, max on right) or vertically (min on bottom, max on top).'),
      '#default_value' => $settings['orientation']
    );
    $form['jslider_settings']['range'] = array(
      '#type' => 'select',
      '#title' => t('Range'),
      '#options' => array(
        FALSE => t('Disable'),
        TRUE => t('Auto'),
        'min' => t('Minimum'),
        'max' => t('Maximum')
      ),
      '#description' => t('Whether the slider represents a range.'),
      '#default_value' => $settings['range']
    );
    $form['jslider_settings']['step'] = array(
      '#type' => 'textfield',
      '#title' => t('Step'),
      '#size' => 5,
      '#description' => t('Determines the size or amount of each interval or step the slider takes between the min and max. The full specified value range of the slider (max - min) should be evenly divisible by the step.'),
      '#required' => TRUE,
      '#element_validate' => array('element_validate_integer_positive'),
      '#default_value' => $settings['step']
    );
    $form['jslider_settings']['slider_style'] = array(
      '#type' => 'select',
      '#title' => t('Style Style'),
      '#options' => array(
        '' => t('Default'),
        'red' => t('Red'),
        'green' => t('Green'),
        'blue' => t('Blue'),
      ),
      '#description' => t('Some default color styles for ease of use'),
      '#default_value' => $settings['slider_style']
    );
    $form['jslider_settings']['display_values'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display Values'),
      '#description' => t('If enabled display the current values of slider as simple text.'),
      '#default_value' => $settings['display_values']
    );
    $form['jslider_settings']['slider_length'] = array(
      '#type' => 'textfield',
      '#title' => t('Slider Length'),
      '#size' => 5,
      '#description' => t('Acceptable types are the same as css width and height (100px) and it will be used as width or height depending on #orientation'),
      '#default_value' => $settings['slider_length']
    );
    $form['jslider_settings']['hide_inputs'] = array(
      '#type' => 'checkbox',
      '#title' => t('Hide Input Textfields'),
      '#description' => t('If enabled displays only the slider and hides input textfields.'),
      '#default_value' => $settings['hide_inputs']
    );
  }

  return $form;
}

/**
 * @param $form
 * @param $form_state
 * @param $field
 * @param $instance
 * @param $langcode
 * @param $items
 * @param $delta
 * @param $element
 * @return mixed
 */
function _jslider_field_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  switch ($instance['widget']['type']) {
    case 'jslider':
      $settings = $instance['widget']['settings']['jslider_settings'];
      $element[$element['#field_name'] . '_jslider'] = array(
        '#type' => 'jslider',
        '#input_title' => NULL,
        '#required' => $instance['required'],
        '#input2_title' => t('Max'),
        '#animate' => $settings['animate'],
        '#disabled' => FALSE,
        '#max' => $instance['settings']['max'],
        '#min' => $instance['settings']['min'],
        '#orientation' => $settings['orientation'],
        '#range' => $settings['range'],
        '#step' => $settings['step'],
        '#value' => 0,
        '#value2' => NULL, //When using range
        '#slider_style' => $settings['slider_style'],
        '#size' => 3,
        '#display_inputs' => !$settings['hide_inputs'],
        '#display_values' => $settings['display_values'],
        '#slider_length' => $settings['slider_length'],
        '#display_inside_fieldset' => FALSE
      );
      break;
  }
  return $element;
}