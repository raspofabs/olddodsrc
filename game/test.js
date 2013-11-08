function Foo() { this.a; this.inc = function() { this.a = this.a + 1; }; }
var c = new Foo();
c.a = 4;
c.inc();
c.a;
print(c.a);
print("hello from inside Javascript");

