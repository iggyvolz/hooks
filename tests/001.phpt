--TEST--
Basic functionality of hooks
--SKIPIF--
<?php
if (!extension_loaded('hooks')) die('skip: hooks extension required');
?>
--FILE--
<?php
#[Attribute]
class x implements PreHook, PostHook{
    public function __construct(){echo "New x\n";}
    public function before(string $method, null|object|string $target, array $params): void
    {
        var_dump(["type" => "before", "method" => $method, "object" => $target, "params" => $params]);
    }
    public function after(string $method, null|object|string $target, mixed $retval): void
    {
        var_dump(["type" => "after", "method" => $method, "object" => $target, "retval" => $retval]);
    }
}
#[x]
function foo(){echo "(foo)\n";}
foo();
class bar
{
    #[x]
    public function do(int $a): int {
        echo "(do)\n";
        return 123;
    }
    #[x]
    public static function doStatic(string $b, array $c): int {
        echo "(doStatic)\n";
        return 456;
    }
}
$y = new bar();
$y->do(2);
bar::doStatic("x", [2,null]);
?>
--EXPECT--
array(4) {
  ["type"]=>
  string(6) "before"
  ["method"]=>
  string(3) "foo"
  ["object"]=>
  NULL
  ["params"]=>
  array(0) {
  }
}
(foo)
array(4) {
  ["type"]=>
  string(5) "after"
  ["method"]=>
  string(3) "foo"
  ["object"]=>
  NULL
  ["retval"]=>
  NULL
}
array(4) {
  ["type"]=>
  string(6) "before"
  ["method"]=>
  string(2) "do"
  ["object"]=>
  object(bar)#3 (0) {
  }
  ["params"]=>
  array(1) {
    [0]=>
    int(2)
  }
}
(do)
array(4) {
  ["type"]=>
  string(5) "after"
  ["method"]=>
  string(2) "do"
  ["object"]=>
  object(bar)#3 (0) {
  }
  ["retval"]=>
  int(123)
}
array(4) {
  ["type"]=>
  string(6) "before"
  ["method"]=>
  string(8) "doStatic"
  ["object"]=>
  string(3) "bar"
  ["params"]=>
  array(2) {
    [0]=>
    string(1) "x"
    [1]=>
    array(2) {
      [0]=>
      int(2)
      [1]=>
      NULL
    }
  }
}
(doStatic)
array(4) {
  ["type"]=>
  string(5) "after"
  ["method"]=>
  string(8) "doStatic"
  ["object"]=>
  string(3) "bar"
  ["retval"]=>
  int(456)
}
