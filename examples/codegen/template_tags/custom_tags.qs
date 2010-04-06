

function SimplifyNode()
{
  this.render = function( context )
  {
    var s = context.render(this.nodeList);
    // TODO: Simplify
    return s;
  }
}

function SimplifyNodeFactory( tagContent, parser )
{
  var node = new Node("SimplifyNode");

  var nodeList = parser.parse( node, [ "endsimplify" ] );
  node.setNodeList("nodeList", nodeList);
  return node;
}
Library.addFactory("SimplifyNodeFactory", "simplify");

function RandomNode( numChars, contextName )
{
  this.numChars = numChars;
  this.contextName = contextName;

  this.render = function( context )
  {
    var num = this.numChars.resolve(context);
    var num = parseInt(Math.random() * (Math.pow(10, this.numChars.resolve(context) )));
    context.insert(this.contextName, num);
  }
}

function RandomNodeFactory( tagContent, parser )
{
  var tagArgs = AbstractNodeFactory.smartSplit(tagContent);
  var numChars = FilterExpression(tagArgs[1]);
  var contextName = tagArgs[3];
  var node = new Node("RandomNode", numChars, contextName);
  return node;
}
Library.addFactory("RandomNodeFactory", "random");

var ToWriteFilter = function(input)
{
  input = input.rawString();
  return "set" + input[0].toUpperCase() + input.slice(1);
};
ToWriteFilter.filterName = "to_write";
ToWriteFilter.isSafe = true;
Library.addFilter("ToWriteFilter");

var ToArgFilter = function(input)
{
  input = input.rawString();
  if (input.substr(input.length - 1, input.length) == "*")
    return input;

  if (input.substr(0, 1) != "Q")
    return input + " ";

  return "const " + input + " &";
};
ToArgFilter.filterName = "to_arg";
ToArgFilter.isSafe = true;
Library.addFilter("ToArgFilter");

var ToPublicFilter = function(input)
{
  input = input.rawString();
  input = input.replace("Private", "");
  return input;
};
ToPublicFilter.filterName = "to_public";
ToPublicFilter.isSafe = true;
Library.addFilter("ToPublicFilter");


var AppendFilter = function(input, arg)
{
  input = input.rawString();
  arg = arg.rawString();

  return input + arg;
};
AppendFilter.filterName = "append";
AppendFilter.isSafe = true;
Library.addFilter("AppendFilter");
