<html>
<head>
<title>title</title>
</head>

<body>


  <form action="/rc" method="post" enctype="multipart/form-data">
<p>
   <button type = "submit" 
   % if state == 'running':
			disabled	
   % end
    name="start" value="true">
    Start
  </button>
</p>
<p>
  <button type = "submit"
   % if state == 'ready':
			 disabled
   % end
    name="stop" value="true">
     Stop
  </button>
</p>

<p>
  <button type = "submit"
    name="abort" value="true">
     Abort
  </button>
</p>

  </form>


</body>
