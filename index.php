<?php session_start();
	if(!isset($_SESSION['UserData']['Username'])){
		header("location:login.php");
		exit;
	}
?>
<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1">
	<link rel="stylesheet" type="text/css" href="css/w3.css">
	<title>App</title>
	<style>
		.pad {
			padding: 5px;
		}
	</style>
</head>
<body>
	<div class="w3-container w3-blue-grey w3-padding">
		<div class="pad">
			<button class="w3-btn w3-white w3-border w3-border-red w3-round-large" onclick="window.location.href='http://192.168.43.234'">Home Appliances</button>
			<button class="w3-btn w3-white w3-border w3-border-red w3-round-large" onclick="window.location.href='gate.php'">Front Gate</button>
		</div>
		<div>
			<button class="w3-btn w3-white w3-border w3-border-red w3-round-large" onclick="window.location.href='logout.php'">Log Out</button>
		</div>
	</div>
</body>
</html>
