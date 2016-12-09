<?php

// echo (int)isset($_GET['key']);
// echo (int)( ! empty($_GET['key']));
// echo (int)in_array($_GET['key'], ['T', 'L', 'R', 'N']);

define("DATA_FILE", "data.txt");

$validActions = ['T', 'L', 'R', 'N'];

$action = $_GET['key'];
$hasKey = ! is_null($action) && in_array($action, $validActions);

// read current contents
$response = file_get_contents(DATA_FILE, true);
$resData = explode(':', $response);
$resActions = $resData[1];

// append new contents
if ($hasKey) {
  $contents = '';

  switch ($action) {
    case 'T': case 'L': case 'R':
      $contents = time() . ':' . $resActions . $action;
      break;
  }

  file_put_contents(DATA_FILE, $contents);
}

// count actions individually
$top_val = substr_count($resActions, 'T');
$left_val = substr_count($resActions, 'L');
$right_val = substr_count($resActions, 'R');

// ...

?>
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">

  <?php if ( ! $hasKey ): ?>
  <!-- <meta http-equiv="refresh" content="3"> -->
  <?php endif; ?>

  <title>Reverse Simon Says</title>

  <style>
    body {
      width: 100%;
      height: 100%;
      background: #eee;
      font-family: "Helvetica Neue", Helvetica, sans-serif;
    }

    .circle {
      width: 100px;
      height: 100px;
      border-radius: 50px;
      background: #ccc;
      border: 1px solid #888;
      position: fixed;
      text-align: center;
    }
    .circle .count {
      font-size: 2rem;
      padding-top: 29px;
    }

    #reload {
      background: transparent;
      border: 1px solid #222;
      border-radius: 10px;
      cursor: pointer;
      outline: none;
    }
    #reload:hover {
      background: #222;
      color: #eee;
    }

    #top {
      top: 100px;
      left: 50%;
      margin-left: -50px;
      background: rgb(13, 223, 13);
    }
    #left {
      top: 230px;
      left: 25%;
      margin-left: -50px;
      background: rgb(223, 223, 13);
    }
    #right {
      top: 230px;
      right: 25%;
      margin-right: -50px;
      background: rgb(223, 13, 13);
    }
  </style>
</head>
<body id="root" data-response="<?php echo $response; ?>">
  <button id="reload" onclick="reload()">reload</button>

  <div id="top" class="circle">
    <div class="count"><?php echo $top_val; ?></div>
  </div>
  <div id="left" class="circle">
    <div class="count"><?php echo $left_val; ?></div>
  </div>
  <div id="right" class="circle">
    <div class="count"><?php echo $right_val; ?></div>
  </div>

  <script>
    var root = document.querySelector('#root');
    console.debug('root: ', root.getAttribute('data-response'));

    function reload() {
      location.reload();
    }
  </script>
</body>
</html>
