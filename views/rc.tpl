<html>
<head>
<title>title</title>
</head>

<body>

<div>
<p>
 <form action="/init" method="post">


% for num, addrip in enumerate(ip, start=0):
  <input type ="text" 
  % if state != 'abort':
	 disabled
  % end
  name={{'ip%s' % (num)}} value={{addrip}}> <input type="checkbox" 
  % if state != 'abort':
         disabled
  % end
  name={{'check%s' % (num)}} /> 
% end
<input value="Go!" 
  % if state != 'abort':
         disabled
  % end
  type="submit" />
</form>
</p>

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
</div>

</body>
