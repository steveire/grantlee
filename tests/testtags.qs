

var EchoNode = function(content)
{
  this.content = content;
  this.render = function(context)
  {
    return content.join(" ");
  };
};

function EchoNodeFactory(tagContent, parser, parent)
{
  var content = tagContent.split(" ");
  content = content.slice(1, content.length);
  return new Node("EchoNode", content);
};
EchoNodeFactory.tagName = "echo";
Library.addFactory("EchoNodeFactory");