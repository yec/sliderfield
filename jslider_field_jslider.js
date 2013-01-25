(function($, Drupal) {
  /**
   * This script adds jQuery slider functionality to transform_slider form element
   */
  Drupal.behaviors.jSliderFieldjSlider = {
    attach: function (context, settings) {

      // Create sliders
      $('.jslider-container:not(.jslider-processed)', context).each(function () {

        $(this).addClass('jslider-processed');
        var slider_id = $(this).parent().attr('id');
        console.log(settings);
        console.log(slider_id);
        // Get values
        var $slider = $(this).parents('.jslider', context);

        var $values = [];
        var $value = $slider.find('.jslider-value-field', context).val() - 0;
        var $value2 = $slider.find('.jslider-value2-field', context).val() - 0;
        if (!isNaN($value2)) {
          $values = [$value, $value2];
          $slider.find('.jslider-display-values-field').html($values.join(' - '));
        } else {
          $slider.find('.jslider-display-values-field').html($value);
        }

        var setting = settings['jslider_field_' + slider_id];

        if (!setting.display_inputs) {
          $slider.find('.jslider-value-field, .jslider-value2-field', context).parent().hide();
        }
        // Setup slider
        $(this).slider({
          value: $value,
          animate : setting.animate,
          max : setting.max - 0,
          min : setting.min - 0,
          orientation : setting.orientation,
          range : setting.range,
          step : setting.step,
          values : $values,
          slide: jslidersSlideProcess,
          change: jslidersSlideProcess
        });

        if (setting.disabled) {
          $(this).slider( "disable" );
        }
      });



      // Bind left textfield changes
      $('.jslider-value-field:not(.jslider-processed)', context)
          .addClass('jslider-processed')
          .keyup(function(e) {
            // Get container
            var $slider = $(this).parents('.jslider', context);

            // Left input value
            var $value = $(this).val() - 0;
            if (isNaN($value)) {
              $value = 0;
              $slider.find('.jslider-value-field', context).val($value);
            }

            // Get slider max value
            var $jSlider = $slider.find('.jslider-container', context);
            var $max = $jSlider.slider('option', 'max');

            // Validate left input
            if ($value > $max) {
              $value = $max;
              $slider.find('.jslider-value-field', context).val($value);
            }

            // Setup right value
            //$slider.find('.jslider-right-field', context).val($max - $left);

            // Move slider without toggling events
            $jSlider.slider({value: $value});
          });

      // Bind left textfield changes
      $('.jslider-value2-field:not(.jslider-processed)', context)
          .addClass('jslider-processed')
          .keyup(function(e) {

            // Get container
            var $slider = $(this).parents('.jslider', context);

            // Left input value
            var $value = $(this).val() - 0;
            if (isNaN($value)) {
              $value = 0;
              $slider.find('.jslider-value2-field', context).val($value);
            }

            // Get slider max value
            var $jSlider = $slider.find('.jslider-container', context);
            var $max = $jSlider.slider('option', 'max');

            // Validate left input
            if ($value > $max) {
              $value = $max;
              $slider.find('.jslider-value2-field', context).val($value);
            }

            // Setup right value
            //$slider.find('.jslider-right-field', context).val($max - $left);

            // Move slider without toggling events
            $jSlider.slider('values', 1, $value);
          });
    }
  }

  // Slider processor
  var jslidersSlideProcess = function(event, ui) {
    // Setup values
    var $slider = $(this).parents('.jslider');
    var $values = [];
    if ($slider.find('.jslider-value2-field').length > 0) {
      $slider.find('.jslider-value-field').val(ui.values[0]);
      $slider.find('.jslider-value2-field').val(ui.values[1]);
      $values = ui.values;
    } else {
      $slider.find('.jslider-value-field').val(ui.value);
      $values.push(ui.value);
    }
    $slider.find('.jslider-display-values-field').html($values.join(' - '));
  }

})(jQuery, Drupal);
