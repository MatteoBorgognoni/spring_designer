<?php

/**
 * @file
 * Spring Designer module.
 */


function spring_designer_form($form, &$form_state, $type) {
  $form = array();

  $form['#prefix'] = '<div id="spring-designer-form-wrapper">';
  $form['#suffix'] = '</div>';

  $form['spring_designer_type'] = array(
    '#type' => 'hidden',
    '#title' => t('Spring Type'),
    '#value' => $type,
  );

  $form['spring_designer_units'] = array(
    '#type' => 'select',
    '#title' => t('Units'),
    '#options' => array(
        'metric' => 'Metric',
        'imperial' => 'Imperial',
      ),
    '#attributes' => array(
        'class' => array(
          'product-unit-select',
        ),
      ),
  );
  
  $form['spring_designer_material'] = array(
    '#type' => 'select',
    '#title' => t('Material'),
    '#options' => array(
        'M' => 'Carbon Steel',
        'S' => 'Stainless Steel',
      ),
  );

  $diameter_options = array();
  switch ($type) {
    case 'compression':
      $diameter_options = array(
        'outside' => 'Outside',
        'mean' => 'Mean',
        'inside' => 'Inside',
      );
      break;
    case 'extension':
      $diameter_options = array(
        'outside' => 'Outside',
      );
      break;
  }

  $form['spring_designer_diameter_type'] = array(
    '#type' => 'select',
    '#title' => t('Spring diameter'),
    '#options' => $diameter_options,
  );

  $form['spring_designer_diameter_value'] = array(
    '#type' => 'textfield',
    '#size' => 5,
  );

  $form['spring_designer_free_length'] = array(
    '#title' => t('Free Length'),
    '#type' => 'textfield',
    '#size' => 5,
  );


  $form['submit'] = array(
    '#type' => 'submit',
    '#name' => 'search',
    '#value' => t('Search'), 
    '#id' => 'search-submit',
    '#ajax' => array(
      'callback' => 'spring_designer_ajax_callback',
      'method' => 'replace',
      'wrapper' => 'spring-designer-form-wrapper',
    ),
  );    

  $springs_table = isset($form_state['session']['calculated']) ? $form_state['session']['calculated'] : '';


  $form['spring_designer_table_wrapper'] = array(
    // The prefix/suffix provide the div that we're replacing, named by
    // #ajax['wrapper'] above.
    '#markup' => $springs_table,
    '#prefix' => '<div id="spring-designer-table-wrapper">',
    '#suffix' => '</div>',
  );

  return $form;
}



function spring_designer_ajax_callback($form, &$form_state) {

  return $form;

}


function spring_designer_form_validate($form, &$form_state) {

  $values = $form_state['values'];
  $errors = variable_get('spring_designer_errors');

  // Diameters
  if (!is_numeric($values['spring_designer_diameter_value'])) {
    form_set_error('spring_designer_diameter_value', 'Please enter a numeric value');
  }
  // Free Length
  if (!is_numeric($values['spring_designer_free_length'])) {
    form_set_error('spring_designer_diameter_value', 'Please enter a numeric value');
  }

  // No diameter field or free length field can be left empty
  if (empty($values['spring_designer_diameter_value'])) {
    form_set_error('spring_designer_diameter_value', $errors['error_1']);
  }
  if (empty($values['spring_designer_free_length'])) {
    form_set_error('spring_designer_free_length', $errors['error_1']);
  }
 
  // Set error for values which exceed limits

  $type = $values['spring_designer_type'];
  $diameter_value = number_format($values['spring_designer_diameter_value'], 1, '.', '');
  $free_length = number_format($values['spring_designer_free_length'], 1, '.', '');

  $min_od = variable_get('spring_designer_od_min');
  $max_od = variable_get('spring_designer_od_max');
  $max_fl = variable_get('spring_designer_fl_max');
  $min_fl_od_compression = variable_get('spring_designer_compression_fl_od_min');
  $max_fl_od_compression = variable_get('spring_designer_compression_fl_od_max');
  $min_fl_od_extension = variable_get('spring_designer_extension_fl_od_min');
  $max_fl_od_extension = variable_get('spring_designer_extension_fl_od_max');

/*
  if ($diameter_value < $min_od || $diameter_value > $max_od) {
    form_set_error('spring_designer_diameter_value', $errors['error_2']);
  }
  if ($free_length > $max_fl) {
    form_set_error('spring_designer_free_length', $errors['error_2']);
  }

  if ($type == 'compression') {
    if (($free_length / $diameter_value) < $min_fl_od_compression || ($free_length / $diameter_value) > $max_fl_od_compression) {
      form_set_error('spring_designer_free_length', $errors['error_3']);
    } 
  } 

  if ($type == 'extension') {
    if (($free_length / $diameter_value) < $min_fl_od_extension || ($free_length / $diameter_value) > $max_fl_od_extension) {
      form_set_error('spring_designer_free_length', $errors['error_3']);
    } 
  } 
  */
 
   if ($type == 'compression') {
    if (($free_length / $diameter_value) < $min_fl_od_compression || 
        ($free_length / $diameter_value) > $max_fl_od_compression ||
        ($diameter_value < $min_od) || 
        ($diameter_value > $max_od) ||
        ($free_length > $max_fl)) {
      form_set_error('spring_designer_free_length', $errors['error_1']);
    } 
  } 

  if ($type == 'extension') {
    if (($free_length / $diameter_value) < $min_fl_od_extension || 
        ($free_length / $diameter_value) > $max_fl_od_extension ||
        ($diameter_value < $min_od) || 
        ($diameter_value > $max_od) ||
        ($free_length > $max_fl)) {
      form_set_error('spring_designer_free_length', $errors['error_1']);
    } 
  } 
}

/**
 * spring_designer submit handler.
 */
function spring_designer_form_submit($form, &$form_state) {
  // Rebuild the form in order to keep filter values.
  $values = $form_state['values'];
  $form_state['rebuild'] = TRUE;

  $type = $values['spring_designer_type'];
  $units = $values['spring_designer_units'];
  $material = $values['spring_designer_material'];
  $diameter_type = $values['spring_designer_diameter_type'];
  $diameter_value = number_format($values['spring_designer_diameter_value'], 1, '.', '');
  $free_length = number_format($values['spring_designer_free_length'], 1, '.', '');

  $table = spring_designer_calculate_attributes($type, $units, $material, $diameter_type, $diameter_value, $free_length);
  $empty_result = '<div class="spring-designer-empty">' . variable_get('spring_designer_empty_result') . '</div>';

  $form_state['session']['calculated'] = !empty($table) ? $table : $empty_result;
}


function spring_designer_single_form($form, &$form_state, $type) {
  $form = array();

  $form['spring_designer_single_type'] = array(
    '#type' => 'hidden',
    '#title' => t('Spring Type'),
    '#value' => $type,
  );

  $length_options = array();
  switch ($type) {
    case 'compression':
      $length_options = array(
        'A',
        'B',
      );
      $type_string = 'FC - ';
      break;
    case 'extension':
      $length_options = array(
        'T',
        'V',
      );
      $type_string = 'FX - ';
      break;
  }

  $form['spring_designer_single_initial_string'] = array(
    '#type' => 'markup',
    '#markup' => $type_string,
    '#prefix' => '<span class="form-item spring-designer-single-type-string">',
    '#suffix' => '</span>',
  );

  $form['spring_designer_single_length'] = array(
    '#type' => 'select',
    '#options' => $length_options,
    //'#description' => 'Select "' . $length_options[0] . '" for springs shorter than 50mm or "' . $length_options[1] . '" for springs longer than 50mm',
  );

  $form['spring_designer_single_diameter_value'] = array(
    '#type' => 'textfield',
    '#attributes' => array('maxlength' => 4, 'size' => 4), 
  );

  $form['spring_designer_single_length_value'] = array(
    '#type' => 'textfield',    
    '#attributes' => array('maxlength' => 4, 'size' => 4), 
  );

  $ws = array();
  $wire_sizes_options = explode(PHP_EOL, variable_get('spring_designer_admin_wire_sizes'));
  foreach ($wire_sizes_options as $key => $wire_size_option) {
    $wire_size = explode(',', $wire_size_option);
    $ws[$wire_size[0]] = $wire_size[0]; // or - number_format($wire_size[1], 2);
  }

  $form['spring_designer_single_wire_size_value'] = array(
    '#type' => 'select',
    '#options' => $ws,
    //'#description' => 'Choose a Wire Size option',
  );

  if ($type == 'compression') {
    $solid_ratings = explode(PHP_EOL, variable_get('spring_designer_admin_solid_stress'));
    $ratings = array();
    foreach ($solid_ratings as $key => $value) {
      $ratings[$key] = $key + 1; 
    }
    $form['spring_designer_single_solid_stress'] = array(
      '#type' => 'select',
      '#options' => $ratings,
    );
  }

  $form['spring_designer_single_material'] = array(
    '#type' => 'select',
    '#options' => array(
        'M' => 'M',
        'S' => 'S',
      ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#name' => 'search-single',
    '#value' => t('Search'), 
    '#id' => 'search-single-submit',
    '#ajax' => array(
      'callback' => 'spring_designer_single_ajax_callback',
      'method' => 'replace',
      'wrapper' => 'spring-designer-single-form-wrapper',
    ),
  );    

  $springs_table = isset($form_state['session']['single_calculated']) ? $form_state['session']['single_calculated'] : '';


  $form['spring_designer_single_table_wrapper'] = array(
    // The prefix/suffix provide the div that we're replacing, named by
    // #ajax['wrapper'] above.
    '#markup' => $springs_table,
    '#prefix' => '<div id="spring-designer-table-wrapper">',
    '#suffix' => '</div>',
  );
  return $form;
}


function spring_designer_single_ajax_callback($form, &$form_state) {

  return $form;

}



function spring_designer_single_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  $errors = variable_get('spring_designer_errors');
  $type = $values['spring_designer_single_type'];
  $letter = $values['spring_designer_single_length'];
  $diameter_value = number_format(($values['spring_designer_single_diameter_value'] * 0.05), 1, '.', '');
  $free_length = ''; 
  if ($letter == 0) {
    $free_length = $values['spring_designer_single_length_value'] * 0.05;
  } else {
    $free_length = $values['spring_designer_single_length_value'] * 0.5;
  }
  $free_length = number_format($free_length, 1, '.', '');

  // No diameter field or free length field can be left empty
  if (empty($values['spring_designer_single_diameter_value'])) {
    form_set_error('spring_designer_single_diameter_value', $errors['error_1']);
  }
  if (empty($values['spring_designer_single_length_value'])) {
    form_set_error('spring_designer_single_length_value', $errors['error_1']);
  }
 
  // Set error for values which exceed limits
  $min_od = variable_get('spring_designer_od_min');
  $max_od = variable_get('spring_designer_od_max');
  $max_fl = variable_get('spring_designer_fl_max');
  $min_fl_od_compression = variable_get('spring_designer_compression_fl_od_min');
  $max_fl_od_compression = variable_get('spring_designer_compression_fl_od_max');
  $min_fl_od_extension = variable_get('spring_designer_extension_fl_od_min');
  $max_fl_od_extension = variable_get('spring_designer_extension_fl_od_max');
/*
  if ($diameter_value < $min_od || $diameter_value > $max_od) {
    form_set_error('spring_designer_single_diameter_value', $errors['error_2']);
  }

  if ($free_length > $max_fl) {
    form_set_error('spring_designer_single_length_value', $errors['error_2']);
  }
  if ($type == 'compression') {
    //dsm($free_length . ' / ' . $diameter_value . ' = ' . $free_length / $diameter_value);
    if (($free_length / $diameter_value) < $min_fl_od_compression || ($free_length / $diameter_value) > $max_fl_od_compression) {
      form_set_error('spring_designer_single_length_value', $errors['error_3']);
    } 
  } 

  if ($type == 'extension') {
    //dsm($free_length . ' / ' . $diameter_value . ' = ' . $free_length / $diameter_value);
    if (($free_length / $diameter_value) < $min_fl_od_extension || ($free_length / $diameter_value) > $max_fl_od_extension) {
      form_set_error('spring_designer_single_diameter_value', $errors['error_3']);
    } 
  } 
  */
 
  if ($type == 'compression') {
    //dsm($free_length . ' / ' . $diameter_value . ' = ' . $free_length / $diameter_value);
    if (($free_length / $diameter_value) < $min_fl_od_compression || 
        ($free_length / $diameter_value) > $max_fl_od_compression ||
        ($diameter_value < $min_od || $diameter_value > $max_od) ||
        ($free_length > $max_fl)) {
      form_set_error('spring_designer_single_length_value', variable_get('spring_designer_empty_result'));
    } 
  } 

  if ($type == 'extension') {
    //dsm($free_length . ' / ' . $diameter_value . ' = ' . $free_length / $diameter_value);
    if (($free_length / $diameter_value) < $min_fl_od_extension || 
        ($free_length / $diameter_value) > $max_fl_od_extension ||
        ($diameter_value < $min_od || $diameter_value > $max_od) ||
        ($free_length > $max_fl)) {
      form_set_error('spring_designer_single_diameter_value', variable_get('spring_designer_empty_result'));
    } 
  } 

  //form_set_error('test', 'test');
}

/**
 * spring_designer submit handler.
 */
function spring_designer_single_form_submit($form, &$form_state) {
  // Rebuild the form in order to keep filter values.
  $values = $form_state['values'];
  $form_state['rebuild'] = TRUE;

  $type = $values['spring_designer_single_type'];
  $letter = $values['spring_designer_single_length'];
  $units = 'metric';
  $material = $values['spring_designer_single_material'];
  $diameter_type = 'outside';
  $diameter_value = number_format(($values['spring_designer_single_diameter_value'] * 0.05), 1, '.', '');
  $free_length = ''; 
  if ($letter == 0) {
    $free_length = $values['spring_designer_single_length_value'] * 0.05;
  } else {
    $free_length = $values['spring_designer_single_length_value'] * 0.5;
  }
  $free_length = number_format($free_length, 1, '.', '');
  $wire_size = $values['spring_designer_single_wire_size_value'];
  $stress_rating = $values['spring_designer_single_solid_stress'];

  $table = spring_designer_calculate_attributes($type, $units, $material, $diameter_type, $diameter_value, $free_length, $wire_size, $stress_rating);
  $empty_result = '<div class="spring-designer-empty">' . variable_get('spring_designer_empty_result') . '</div>';

  $form_state['session']['single_calculated'] = !empty($table) ? $table : $empty_result;
 
}
