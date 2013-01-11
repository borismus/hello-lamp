var se = new SpeechEngine({autorestart: true});
var lamp = new LampController('http://lamp-server.appspot.com');

var cmdEl = document.querySelector('#command');

se.onResult(onSpeech);
se.start();

lamp.onChange(onLampChange);

var isListening = false;
window.addEventListener('keydown', onKeyDown);

function onSpeech(e) {
  var commandEl = document.querySelector('#command');
  if (e.isFinal) {
    commandEl.innerText = e.transcript;
    if (e.transcript.indexOf('on') >= 0) {
      lamp.on();
    } else if (e.transcript.indexOf('off') >= 0) {
      lamp.off();
    }
  }
}

function onLampChange(e) {
  var text = e ? 'On :)' : 'Off :(';
  var lampEl = document.querySelector('#lamp');
  var switchEl = document.querySelector('#switch');
  if (e) {
    lampEl.classList.add('on');
    switchEl.classList.add('on');
  } else {
    lampEl.classList.remove('on');
    switchEl.classList.remove('on');
  }
}
