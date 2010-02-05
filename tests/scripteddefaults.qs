
function IfNode( truthVariable )
{
  this.truthVariable = truthVariable;

  this.render = function( context )
  {
    if ( this.truthVariable.isTrue( context ) )
    {
      return context.render(this.trueList);
    } else {
      return context.render(this.falseList);
    }
  }
}


function IfNodeFactory( tagContent, parser )
{
  // TODO: Make smartSplit globally available standalone
  var tagArgs = AbstractNodeFactory.smartSplit(tagContent);
  var truthVariable = FilterExpression(tagArgs[1]);
  var falseList;

  var node = new Node("IfNode", truthVariable);

  var trueList = parser.parse( node, [ "else", "endif2" ] );
  node.setNodeList("trueList", trueList);
  if (parser.nextToken().content == "else")
  {
    falseList =  parser.parse( node, "endif2" );
    node.setNodeList("falseList", falseList);
    parser.deleteNextToken();
  }
  return node;
}
Library.addFactory("IfNodeFactory", "if2");


function IfEqualNode(expression1, expression2 ){
  this.expression1 = expression1;
  this.expression2 = expression2;
  this.render = function( context )
  {
    if (expression1.equals(expression2 , context))
    {
      return context.render(this.trueList);
    } else {
      return context.render(this.falseList);
    }
  }
}

function IfEqualNodeFactory(tagContent, parser)
{
  var tagArgs = AbstractNodeFactory.smartSplit(tagContent);
  var expression1 = FilterExpression(tagArgs[1], parser);
  var expression2 = FilterExpression(tagArgs[2], parser);

  var node = new Node("IfEqualNode", expression1, expression2 );
  var trueList = parser.parse(node, ["else", "endifequal2"]);
  node.setNodeList("trueList", trueList);
  if (parser.nextToken().content == "else")
  {
    falseList = parser.parse(node, "endifequal2");
    node.setNodeList("falseList", falseList);
    parser.deleteNextToken();
  }
  return node;
}
Library.addFactory("IfEqualNodeFactory", "ifequal2");


var SafeFilter = function(input)
{
  return mark_safe(input.rawString());
};
SafeFilter.filterName = "safe2";
SafeFilter.isSafe = true;
Library.addFilter("SafeFilter");

var UpperFilter = function(input)
{
  // Duck-cast it to a string
  input = input.rawString(); // + "";
  return input.toUpperCase();
};
UpperFilter.filterName = "upper";
UpperFilter.isSafe = false;
Library.addFilter("UpperFilter");

var JoinFilter = function(input, filterArgument)
{
  return mark_safe(input.join(filterArgument.rawString()));
};
JoinFilter.filterName = "join2";
JoinFilter.isSafe = true;
Library.addFilter("JoinFilter");
