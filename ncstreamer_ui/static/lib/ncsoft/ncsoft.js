/**
 * Copyright (C) 2017 NCSOFT Corporation
 */


const ncsoft = {};


ncsoft.onDOMContentLoaded = function() {
  var event = new CustomEvent('ncsoftSelectChange');

  // custom select
  $(document).on('click', '.dropdown-menu li a', function(e) {
    var selText = $(this).text();
    $(this).parents('.dropdown').find('.dropdown-toggle').html(
        selText + ' <span class="caret"></span>');
    $(this).parents('.dropdown').find('.dropdown-toggle').val(
        $(this).parent().attr('data-value'));
    document.querySelector(
        '#' + $(this).parents('.dropdown').attr('id')
        ).dispatchEvent(event);
  });

  // tooltip
  $(document).on('click', '.nc-streamer-tooltip .layer-close', function(e) {
    $('.nc-streamer-tooltip').hide();
  });
};
