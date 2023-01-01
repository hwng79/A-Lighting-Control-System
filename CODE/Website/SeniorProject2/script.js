var firebaseConfig = {
    apiKey: "AIzaSyCm2_UPljvIDtAd8mwzXIcZH27o99xESW0",
    authDomain: "sen2-14973.firebaseapp.com",
    databaseURL: "https://sen2-14973-default-rtdb.firebaseio.com",
    projectId: "sen2-14973",
    storageBucket: "sen2-14973.appspot.com",
    messagingSenderId: "394871036757",
    appId: "1:394871036757:web:a9cadd572987a30f41eddc",
    measurementId: "G-TK0BGH42QD"
  };
  // Initialize Firebase
	firebase.initializeApp(firebaseConfig);

$(document).ready(function(){
	
	var database = firebase.database();
	var Led1Status;
	var Led2Status;
	
	database.ref().on("value", function(snap){
		Led1Status = snap.val().Led1Status;
		Led2Status = snap.val().Led2Status;
		
		if(Led1Status == "1"){
			document.getElementById("led").src = "./img/light_bulb.png"
			document.getElementById("unact").style.display = "none";
			document.getElementById("act").style.display = "block";
		} else {
			document.getElementById("led").src = "./img/light_bulb_off.png"
			document.getElementById("unact").style.display = "block";
			document.getElementById("act").style.display = "none";
		}
		if(Led2Status == "1"){
			document.getElementById("led1").src = "./img/ledon.png"
			document.getElementById("unact1").style.display = "none";
			document.getElementById("act1").style.display = "block";
		} else {
			document.getElementById("led1").src = "./img/ledoff.png"
			document.getElementById("unact1").style.display = "block";
			document.getElementById("act1").style.display = "none";
		}
	});

	$(".toggle-btn").click(function(){
		var firebaseRef = firebase.database().ref().child("Led1Status");
		if(Led1Status == "1"){
			firebaseRef.set("0");
			Led1Status = "0";
		} else {
			firebaseRef.set("1");
			Led1Status = "1";
		}
	})
	$(".toggle-btn1").click(function(){
		var firebaseRef = firebase.database().ref().child("Led2Status");
		if(Led2Status == "1"){
			firebaseRef.set("0");
			Led2Status = "0";
		} else {
			firebaseRef.set("1");
			Led2Status = "1";
		}
	})
	database.ref("/LightSen").on("value", function(snapshot) {
		if(snapshot.exists()){
		  var sen = snapshot.val();
		  document.getElementById("lightsensor").innerHTML = sen;
		}
		else
		  console.log("No data available!")
	  });
});