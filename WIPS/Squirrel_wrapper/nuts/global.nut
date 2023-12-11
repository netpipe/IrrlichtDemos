// These are 'global' functions. All scripts may use them.

function setNode(node_id)
{
	si.setCurrentNode(node_id);
}

function getNode()
{
	return si.getCurrentNode();
}

function walk()
{
	local fr = si.getFrameNr();
	if (fr <= 313)
		si.setFrameType(1);
}

function stand()
{
	si.setFrameType(0);
}

function position(node_nr)	// calculate sydney 'grid'
{
	if (node_nr < 1)
		return vector3(0,0,0);

	if (node_nr == 1)
		return vector3(0,0,0);

	local row  = 1;
	local rows = 1;
	local col  = 0;
	
	while (rows <= (node_nr - 0.1))
	{
		row  += 2;
		rows += row;
		col++;
	}
	rows -= row;
	
	return vector3((node_nr - rows * 1.5) * 20.0, col * 20.0, 0);
}
