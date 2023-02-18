import { json } from '@sveltejs/kit';
import fs from 'fs';

/** @type {import('./$types').RequestHandler} */
export async function POST({ request }) {
  const data = await request.formData();
  const file = data.get("audio");
  file.write(`static/audio/${file.name}`)
  fs.writeFileSync(`static/audio/${file.name}`, new Buffer(file));
  return json({ ok: 200 });
}

/** @type {import('./$types').RequestHandler} */
export async function GET({ request }) {
  const lsdir = fs.readdirSync('static/audio');
  return json({ ok: 200, dir: lsdir });
}