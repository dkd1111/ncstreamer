/**
 * Copyright (C) 2017 NCSOFT Corporation
 */


'use strict';


function importScript(file) {
  const elem = document.createElement('script');
  elem.src = file;
  document.body.appendChild(elem);
}


importScript('facebook.js');


const app = {
  dom: {},
  streaming: {
    status: 'standby',  // ['standby', 'starting', 'onAir', 'stopping']
  },
};


document.addEventListener('DOMContentLoaded', function(event) {
  [
    'streaming-sources-select',
    'streaming-status',
  ].forEach(function(domId) {
    app.dom[toCamel(domId)] = document.getElementById(domId);
  });
});


function toCamel(str) {
  return str.replace(/(\-[a-z])/g, function(match) {
    return match.toUpperCase().replace('-', '');
  });
}


function updateStreamingStatus(status) {
  console.info({ status: status });

  app.streaming.status = status;
  app.dom.streamingStatus.textContent = "Status: " + status;
}


function command(cmd, args) {
  let argsArr = [];
  for (const key in args) {
    if (args.hasOwnProperty(key)) {
      argsArr.push([key, encodeURIComponent(args[key])].join('='));
    }
  }
  const uri = ['command://', cmd, '?', argsArr.join('&')].join('');
  location.href = uri;
}


function onClickFacebook() {
  switch (app.streaming.status) {
    case 'standby': {
      updateStreamingStatus('starting');
      facebook.createLiveVideo(function(streamUrl) {
        const source = app.dom.streamingSourcesSelect.value;
        command('streaming/start', {
          serviceProvider: 'Facebook Live',
          streamUrl: streamUrl,
          source: source,
        });
      });
      break;
    }
    case 'onAir': {
      updateStreamingStatus('stopping');
      command('streaming/stop');
      break;
    }
    default: {
      break;
    }
  }
}


function setUpStreamingSources(obj) {
  if (!obj.hasOwnProperty('sources'))
    return;
  for (const source of obj.sources) {
    const option = document.createElement('option');
    option.text = source;
    app.dom.streamingSourcesSelect.add(option);
  }
}


function onStreamingStarted() {
  updateStreamingStatus('onAir');
}


function onStreamingStopped() {
  updateStreamingStatus('standby');
}
