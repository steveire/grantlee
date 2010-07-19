{% for arg in method.args %}{% comment %}
{% endcomment %}{{ arg.name }}{% if default and arg.default %} = {{ arg.default }}{% endif %}{% comment %}
  Add a comma between the arguments, except after the last one:
  {% endcomment %}{% if not forloop.last %}, {% endif %}{% endfor %}
