<rml>
	<head>
		<title>First page</title>
		<link type="text/template" href="window.rml" />
		<style>
			body
			{
				width: 400px;
				height: 325px;

				margin: auto;
			}

			div#title_bar div#icon
			{
				display: none;
			}

			div#content
			{
				text-align: left;
			}
		</style>
	</head>

	<body template="window">
        <p>Third RML</p>
        <p>Custom content for third.rml</p>
         <?php
            echo "<p>Current date: ";
            echo date('Y-m-d H:i:s');
            echo "</p>";
         ?>

        <button onclick="first" id="button-first" type="button">Load first.rml</button> <br></br>
        <button onclick="second" id="button-second" type="button">Load second.rml</button> <br></br>

    </body>
</rml>

