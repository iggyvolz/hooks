--TEST--
Basic functionality of hooks
--SKIPIF--
<?php
if (!extension_loaded('hooks')) die('skip: hooks extension required');
?>
--FILE--
<?php
interface PreHook {
    function before(string $method, null|object|string $target): void;
}
interface PostHook {
    function after(string $method, null|object|string $target, mixed $retval): void;
}
#[Attribute]
class x implements PreHook, PostHook{
    public function before(string $method, null|object|string $target): void
    {
        var_dump(["type" => "before", "method" => $method, "object" => $target]);
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
    public function do(): int {
        echo "(do)\n";
        return 123;
    }
    #[x]
    public static function doStatic(): int {
        echo "(doStatic)\n";
        return 456;
    }
}
$y = new bar();
$y->do();
bar::doStatic();
?>
--EXPECT--
array(2) {
  ["method"]=>
  string(3) "foo"
  ["object"]=>
  NULL
}
(foo)
array(3) {
  ["method"]=>
  string(3) "foo"
  ["object"]=>
  NULL
  ["retval"]=>
  NULL
}
array(2) {
  ["method"]=>
  string(2) "do"
  ["object"]=>
  object(bar)#3 (0) {
  }
}
(do)
array(3) {
  ["method"]=>
  string(2) "do"
  ["object"]=>
  object(bar)#3 (0) {
  }
  ["retval"]=>
  int(123)
}
array(2) {
  ["method"]=>
  string(8) "doStatic"
  ["object"]=>
  string(3) "bar"
}
(doStatic)
array(3) {
  ["method"]=>
  string(8) "doStatic"
  ["object"]=>
  string(3) "bar"
  ["retval"]=>
  int(456)
}
