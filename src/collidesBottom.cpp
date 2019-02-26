
bool collidesBottom(sf::FloatRect collider, sf::FloatRect collided)
{
	return !(collided.left + collided.width < collider.left || collided.left > collider.left + collider.width)
	&& collided.top + collided.height < collider.top + collider.height
	&& collided.top + collided.height >= collider.top;
}