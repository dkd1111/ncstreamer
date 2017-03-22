/**
 * Copyright (C) 2017 NCSOFT Corporation
 */


'use strict';


const app = {
  dom: {},
  streaming: {
    // ['standby', 'setup', 'starting', 'onAir', 'stopping']
    status: 'standby',
    popupBrowserId: 0,
    quality: {
      high: {
        resolution: {
          width: 1280,
          height: 720,
        },
        fps: 30,
        bitrate: 2500,
      },
      medium: {
        resolution: {
          width: 854,
          height: 480,
        },
        fps: 25,
        bitrate: 2000,
      },
      low: {
        resolution: {
          width: 640,
          height: 360,
        },
        fps: 20,
        bitrate: 1500,
      },
    },
  },
};


document.addEventListener('DOMContentLoaded', function(event) {
  [
    'login-page-panel',
    'main-page-panel',
    'streaming-login-button',
    'streaming-feed-description',
    'streaming-sources-select',
    'streaming-status',
    'streaming-control-button',
    'streaming-quality-select',
    'streaming-mic-checkbox',
    'streaming-user-page-select',
    'provider-user-name',
  ].forEach(function(domId) {
    app.dom[toCamel(domId)] = document.getElementById(domId);
  });

  app.dom.streamingLoginButton.addEventListener(
      'click', onStreamingLoginButtonClicked);
  app.dom.streamingControlButton.addEventListener(
      'click', onStreamingControlButtonClicked);
  app.dom.streamingQualitySelect.addEventListener(
      'change', onStreamingQualityChange);
  app.dom.streamingMicCheckbox.addEventListener(
      'change', onMicCheckboxChange);

  setUpSteamingQuality();
});


function toCamel(str) {
  return str.replace(/(\-[a-z])/g, function(match) {
    return match.toUpperCase().replace('-', '');
  });
}


function updateStreamingStatus(status) {
  console.info({ status: status });

  app.streaming.status = status;
  const message = app.dom.streamingStatus;
  const button = app.dom.streamingControlButton;
  switch (status) {
    case 'standby':
      message.textContent = '방송 준비 완료';
      button.textContent = '방송 시작';
      button.disabled = false;
      break;
    case 'setup':
      message.textContent = '방송 생성 중';
      button.textContent = '방송 시작';
      button.disabled = true;
      break;
    case 'starting':
      message.textContent = '방송 시작 중';
      button.textContent = '방송 시작';
      button.disabled = true;
      break;
    case 'onAir':
      message.textContent = '방송 중';
      button.textContent = '방송 종료';
      button.disabled = false;
      break;
    case 'stopping':
      message.textContent = '방송 종료 중';
      button.textContent = '방송 종료';
      button.disabled = true;
      break;
  }
}


function setUpStreamingSources(obj) {
  if (!obj.hasOwnProperty('sources'))
    return;

  for (const child of app.dom.streamingSourcesSelect.children) {
    child.remove();
  }

  if (obj.sources.length == 0) {
    const option = document.createElement('option');
    option.text = '실행 중인 게임이 없습니다.';
    app.dom.streamingSourcesSelect.add(option);
    app.dom.streamingSourcesSelect.disabled = true;
  } else {
    app.dom.streamingSourcesSelect.disabled = false;
    for (const source of obj.sources) {
      const option = document.createElement('option');
      option.value = source;
      option.text = source.split(':')[0];
      app.dom.streamingSourcesSelect.add(option);
    }
  }
}


function setUpSteamingQuality() {
  for (const option of app.dom.streamingQualitySelect) {
    option.remove();
  }

  for (const level in app.streaming.quality) {
    if (!app.streaming.quality.hasOwnProperty(level))
      return;

    const quality = app.streaming.quality[level];
    const option = document.createElement('option');
    option.value = level;
    option.text = [
        level,
        quality.resolution.width + '*' + quality.resolution.height,
        'fps: ' + quality.fps,
        'bitrate: ' + quality.bitrate].join(', ');

    app.dom.streamingQualitySelect.add(option);
  }
  onStreamingQualityChange();
}


function onStreamingLoginButtonClicked() {
  cef.serviceProviderLogIn.request('Facebook Live');
}


function onStreamingControlButtonClicked() {
  ({
    'standby': function() {
      const source = app.dom.streamingSourcesSelect.value;
      const userPage = app.dom.streamingUserPageSelect.value;
      const description = app.dom.streamingFeedDescription.value;
      cef.streamingStart.request(source, userPage, description);
      updateStreamingStatus('starting');
    },
    'starting': function() {},
    'onAir': function() {
      cef.streamingStop.request();
      updateStreamingStatus('stopping');
    },
    'stopping': function() {},
  })[app.streaming.status]();
}


function onStreamingQualityChange() {
  const curValue = app.dom.streamingQualitySelect.value;
  const curQuality = app.streaming.quality[curValue];
  console.info({ streamingQuality: curValue });
  cef.settingsVideoQualityUpdate.request(
      curQuality.resolution.width,
      curQuality.resolution.height,
      curQuality.fps,
      curQuality.bitrate);
}


function onMicCheckboxChange() {
  if (app.dom.streamingMicCheckbox.checked) {
    console.info('mic on');
    cef.settingsMicOn.request();
  } else {
    console.info('mic off');
    cef.settingsMicOff.request();
  }
}


function onBeforePopupOpen(browserId) {
  if (app.streaming.status == 'setup') {
    app.streaming.popupBrowserId = browserId;
  }
}


function OnBeforePopupClose(browserId) {
  if (app.streaming.popupBrowserId == browserId) {
    app.streaming.popupBrowserId = 0;
    updateStreamingStatus('standby');
  }
}


cef.serviceProviderLogIn.onResponse = function(userName, userPages) {
  app.dom.loginPagePanel.style.display = 'none';
  app.dom.mainPagePanel.style.display = 'flex';
  
  for (const child of app.dom.streamingUserPageSelect.children) {
    child.remove();
  }

  for (const userPage of userPages) {
    const option = document.createElement('option');
    option.value = userPage;
    option.text = userPage;
    app.dom.streamingUserPageSelect.add(option);
  }
};


cef.streamingStart.onResponse = function() {
  updateStreamingStatus('onAir');
};


cef.streamingStop.onResponse = function() {
  updateStreamingStatus('standby');
};
