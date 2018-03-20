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
	<title>Front Gate</title>
	<style>
		.pad {
			padding: 5px;
		}
	</style>
</head>
<body>
	<div class="w3-container w3-blue-grey w3-padding w3-center">
		<div class="w3-card-4">
			<img src="Image.jpg" alt="Person that knocked on door">
				<div class="w3-container w3-center" style="width: 100%">
	    			<p>The last person that knocked on door.</p>
  				</div>
		</div>
	</div>
</body>
</html>