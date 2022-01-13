<?php

/** @generate-class-entries */
namespace iggyvolz\hooks;

interface PreHook {
    public function before(string $method, null|object|string $target, array $params): void;
}
interface PostHook {
    public function after(string $method, null|object|string $target, mixed $retval): void;
}