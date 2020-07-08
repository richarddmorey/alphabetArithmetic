
var context=new AudioContext();
var o=null;
var g=null;

var play_correct = function(){
	o=context.createOscillator();
	g=context.createGain();
	o.connect(g);
	o.type="sine";
  o.frequency.value=500;
	g.connect(context.destination);
	o.start(0);
	g.gain.exponentialRampToValueAtTime(0.00001,context.currentTime+0.25);
  o.frequency.exponentialRampToValueAtTime(1000,context.currentTime+0.125);
};


var play_wrong = function(){
	o=context.createOscillator();
	g=context.createGain();
	o.connect(g);
	o.type="sawtooth";
  o.frequency.value=50;
	g.connect(context.destination);
	o.start(0);
	g.gain.exponentialRampToValueAtTime(0.00001,context.currentTime+0.5);
};


var create_stimulus = function(i){
  $("#aa_demo").css("background", "#00006b");
  var addend = (i % 5) + 1;
  var letter = Math.floor( i / 5 ) % 8 + 1;
  var tmp = Math.floor( Math.floor( i / 5 ) / 8 ); 
  var sum = tmp % 3 - 1 + letter + addend;
  
  var stng = String.fromCharCode(64 + letter) + "+" + addend  + "=" + String.fromCharCode(64 + sum);
  var correct = sum == (addend + letter);
  
  $("#stimulus").text(stng);
  $("#true").text(correct);
 
};

var setup_trial = function(e){
  e.preventDefault();
  var istrue = $("#true").text() == "true";
  var press = String.fromCharCode(e.which);
  var correct = (istrue && (press == "z" | press == "Z") ) | (!istrue && press == "/");
  if(correct){
	$("#aa_demo").css("background", "green");
    play_correct();
  }else{
    $("#aa_demo").css("background", "red");
    play_wrong();
  }
  $(document).off("keypress.aa").on("keypress.aa", function(e){
  	e.preventDefault();
  });
  setTimeout(function(){
  	var stim = Math.floor(Math.random()*120);
  	create_stimulus( stim );
    $(document).on("keypress.aa", setup_trial);
  }, 1000);
};
