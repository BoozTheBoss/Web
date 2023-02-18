import { json } from '@sveltejs/kit';
import fs from 'fs';

/** @type {import('./$types').RequestHandler} */
export async function POST({ request }) {
  const data = await request.json();
  console.log(data);
  fs.writeFileSync('static/calli.png', data.image, 'base64');
  return json({ ok: 200 });
}

/** @type {import('./$types').RequestHandler} */
export async function GET({ request }) {
  const lsdir = fs.readdirSync('static');
  return json({ ok: 200, dir: lsdir });
}